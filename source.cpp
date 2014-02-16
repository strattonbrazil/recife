#include "source.h"

#include <iostream>
#include <QStringList>
#include <QVector>
#include <stdexcept>
#include <QFile>
#include <QDir>
#include <opencv2/highgui/highgui.hpp>
#include <QMetaObject>
#include <QMetaProperty>
#include <QHash>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

#include "utils.h"
#include "validator.h"
#include "keyable.h"
#include "layereditor.h"

class FileHandler
{
public:
    QStringList supportedExtensions() { return _supportedExtensions; }
    virtual QSharedPointer<Source> process(QString fileName) = 0;
protected:
    QStringList _supportedExtensions;
};

QHash<QString,LayerEditor*> classEditors;

LayerEditor* Source::editor(FrameContext* frameContext)
{
    const QMetaObject *metaobject = metaObject();
    QString className = metaobject->className();

    if (!classEditors.contains(className)) {
        LayerEditor* editor = new LayerEditor(0, frameContext);
        QGridLayout* layout = new QGridLayout();
        editor->setLayout(layout);
        layout->setMargin(0);

        QMap<QString, QVariant>::iterator i;
        int row = 0;
        for (i = _properties.begin(); i != _properties.end(); ++i) {
            QString name = i.key();
            //QVariant value = i.value();

            QWidget* widget = 0;

            QLineEdit* w = new QLineEdit();
            //w->setReadOnly(true);
            connect(w, SIGNAL(editingFinished()), editor, SLOT(updateTextProperty()));
            widget = w;

            editor->registerInput(name, widget);
            widget->setProperty("variable", name);

            layout->addWidget(new QLabel(name), row, 0);
            layout->addWidget(widget, row, 1);

            QPushButton* button = new QPushButton();
            editor->registerButton(name, button);
            button->setProperty("variable", name);
            setButtonKeyFrame(button, true);
            connect(button, SIGNAL(clicked()), editor, SLOT(setKeyFrame()));
            layout->addWidget(button, row, 2);
            button->setMaximumWidth(30);

            row++;
        }
        classEditors.insert(className, editor);
    }

    return classEditors[className];
}

class ImageHandler : public FileHandler
{
public:
    ImageHandler()
    {
        _supportedExtensions.append("jpg");
        _supportedExtensions.append("png");
    }

    QSharedPointer<Source> process(QString fileName)
    {
        return QSharedPointer<Source>(new ImageSource(fileName));
    }
};

class VideoHandler : public FileHandler
{
public:
    VideoHandler()
    {
        _supportedExtensions.append("mov");
    }

    QSharedPointer<Source> process(QString fileName)
    {
        return QSharedPointer<Source>(new VideoSource(fileName));
    }
};

QVector<FileHandler*> fileHandlers;

void validate()
{
    if (fileHandlers.size() == 0) {
        fileHandlers.append(new ImageHandler());
        fileHandlers.append(new VideoHandler());
    }
}

Mat Source::render(int frame)
{
    Mat base = renderBase(frame);
    Mat processed = _effectsList->process(base, frame);

    return processed;
}

QStringList Source::supportedExtensions()
{
    validate();

    QStringList extensions;
    foreach(FileHandler* handler, fileHandlers) {
        std::cout << "looking at handler..." << std::endl;
        foreach(QString extension, handler->supportedExtensions()) {
            std::cout << "ext: " << extension.toStdString() << std::endl;
            extensions.append(extension);
        }
    }

    //extensions.removeDuplicates();

    return extensions;
}

int idCount = 1;
QSharedPointer<Source> Source::getSource(QString fileName)
{
    validate();

    QFileInfo fileInfo(fileName);

    foreach(FileHandler* handler, fileHandlers) {
        if (handler->supportedExtensions().contains(fileInfo.suffix())) {
            QSharedPointer<Source> src = handler->process(fileName);
            src->_id = idCount++;

            src->_properties.insert(QString("position"), QVariant::fromValue(KeyablePointF()));
            src->_properties.insert(QString("scale"), QVariant::fromValue(KeyablePointF(1,1)));

            src->_effectsList = new EffectsModel();
            return src;
        }
    }

    throw std::runtime_error("failed to find appropriate importer");
}

EffectsModel* Source::effectsModel()
{
    if (_effectsList == 0)
        _effectsList = new EffectsModel();
    return _effectsList;
}

void Source::addEffect(QSharedPointer<Effect> effect)
{
    _effectsList->addEffect(effect);
}

void Source::setKeyFrame(QString propertyName, int frame)
{
    int type = _properties[propertyName].userType();

    if (type == qMetaTypeId<KeyablePointF>()) {
        KeyablePointF kp = _properties[propertyName].value<KeyablePointF>();
        kp.setKeyFrame(frame);
        _properties.insert(propertyName, QVariant::fromValue(kp));
    } else {
        throw std::runtime_error("setKeyFrame: unsupported property type");
    }
}

void Source::removeKeyFrame(QString propertyName, int frame)
{
    int type = _properties[propertyName].userType();

    if (type == qMetaTypeId<KeyablePointF>()) {
        KeyablePointF kp = _properties[propertyName].value<KeyablePointF>();
        kp.removeKeyFrame(frame);
        _properties.insert(propertyName, QVariant::fromValue(kp));
    } else {
        throw std::runtime_error("removeKeyFrame: unsupported property type");
    }
}

void Source::setProperty(QString propertyName, QVariant value)
{
    _properties[propertyName] = value;
    emitUpdate();
}

QMap<QString,QVariant> Source::properties() {
    return _properties;
}

bool Source::hasKeyFrame(int frame)
{
    QMap<QString,QVariant>::Iterator i;
    for (i = _properties.begin(); i != _properties.end(); i++) {
        int type = i.value().userType();

        if (type == qMetaTypeId<KeyablePoint>()) {

        } else if (type == qMetaTypeId<KeyablePointF>()) {
            KeyablePointF kp = i.value().value<KeyablePointF>();
            if (kp.hasKeyFrameAt(frame))
                return true;
        }
    }
    return false;
}

void Source::emitUpdate()
{
    emit(layerChanged(this));
}

ImageSource::ImageSource(QString fileName)
{
    // TODO: use a namebase utility function here
    _label = fileName.split("/").last();
    _imagePath = fileName;
}

Mat ImageSource::renderBase(int frame)
{
    return cv::imread(_imagePath.toStdString(), CV_LOAD_IMAGE_UNCHANGED);
}

VideoSource::VideoSource(QString fileName)
{
    _label = fileName.split("/").last();
    _videoPath = fileName;
}

Mat VideoSource::renderBase(int frame)
{
    /*
    QSharedPointer<CvCapture> video(cvCaptureFromFile(_videoPath.toStdString()));
    IplImage* img = 0;
    if (!cvGrabFrame(video.data())) {
        std::err << "couldn't get frame" << std::endl;
        return;
    }

    img =
      */
    VideoCapture capture(_videoPath.toStdString());

    Mat f;
    for (int i = 0; i < frame; i++) {
        capture >> f;
    }
    return f;
}
