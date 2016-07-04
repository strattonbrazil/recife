#include "layer.h"

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
    virtual QSharedPointer<Layer> process(QString fileName) = 0;
protected:
    QStringList _supportedExtensions;
};

QHash<QString,LayerEditor*> classEditors;

LayerEditor* Layer::editor(TimeContext* frameContext)
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

    QSharedPointer<Layer> process(QString fileName)
    {
        return QSharedPointer<Layer>(new ImageSource(fileName));
    }
};

class VideoHandler : public FileHandler
{
public:
    VideoHandler()
    {
        _supportedExtensions.append("mov");
    }

    QSharedPointer<Layer> process(QString fileName)
    {
        return QSharedPointer<Layer>(new VideoSource(fileName));
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

Mat Layer::render(int frame)
{
    Mat base = renderBase(frame);
    Mat processed = _effectsList->process(base, frame);

    return processed;
}

QStringList Layer::supportedExtensions()
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
QSharedPointer<Layer> Layer::getSource(QString fileName)
{
    validate();

    QFileInfo fileInfo(fileName);

    foreach(FileHandler* handler, fileHandlers) {
        if (handler->supportedExtensions().contains(fileInfo.suffix())) {
            QSharedPointer<Layer> src = handler->process(fileName);
            src->_id = idCount++;

            src->_properties.insert(QString("position"), QVariant::fromValue(KeyablePointF()));
            src->_properties.insert(QString("scale"), QVariant::fromValue(KeyablePointF(1,1)));

            src->_effectsList = new EffectsModel();
            return src;
        }
    }

    throw std::runtime_error("failed to find appropriate importer");
}

EffectsModel* Layer::effectsModel()
{
    if (_effectsList == 0)
        _effectsList = new EffectsModel();
    return _effectsList;
}

void Layer::addEffect(QSharedPointer<Effect> effect)
{
    _effectsList->addEffect(effect);
}

void Layer::setKeyFrame(QString propertyName, int frame)
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

void Layer::removeKeyFrame(QString propertyName, int frame)
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

void Layer::setProperty(QString propertyName, QVariant value)
{
    _properties[propertyName] = value;
    emitUpdate();
}

QMap<QString,QVariant> Layer::properties() {
    return _properties;
}

bool Layer::hasKeyFrame(int frame)
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

void Layer::emitUpdate()
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
    VideoCapture capture(_videoPath.toStdString());
    const int NUM_FRAMES = capture.get(CV_CAP_PROP_FRAME_COUNT);
    const int FRAME_WIDTH = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    const int FRAME_HEIGHT = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

    if (frame > NUM_FRAMES) {
        return Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3, Scalar(0,0,0));
    } else {
        Mat f;
        for (int i = 0; i < frame; i++) {
            capture >> f;
        }
        return f;
    }
}
