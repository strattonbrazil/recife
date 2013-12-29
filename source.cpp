#include "source.h"

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

ImageSource::ImageSource(QString fileName)
{
    // TODO: use a namebase utility function here
    _label = fileName.split("/").last();
    _imagePath = fileName;
}

QString ImageSource::label()
{
    return _label;
}

Mat ImageSource::renderBase(int frame)
{
    return cv::imread(_imagePath.toStdString(), CV_LOAD_IMAGE_UNCHANGED);
}

QHash<QString,LayerEditor*> classEditors;
\
LayerEditor* Source::editor(FrameContext* frameContext)
{
    const QMetaObject *metaobject = metaObject();
    QString className = metaobject->className();

    if (!classEditors.contains(className)) {
        LayerEditor* editor = new LayerEditor(0, frameContext);
        QGridLayout* layout = new QGridLayout();
        editor->setLayout(layout);

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

            QPushButton* button = new QPushButton("K");
            button->setProperty("variable", name);
            connect(button, SIGNAL(clicked()), editor, SLOT(setKeyFrame()));
            layout->addWidget(button, row, 2);
            button->setMaximumWidth(30);

            /*
            if (type == QVariant::nameToType("KeyablePoint")) {

                std::cout << "it is a point" << std::endl;
            }
            else if (type == QVariant::nameToType("KeyablePointF")) {
                std::cout << "it is a pointf" << std::endl;
            }
            std::cout << type << std::endl;
            */
            row++;
        }
        //QMapIterator<QString,QVariant> iter = _properties.iterator();
        /*
        int propertyCount = _properties.count();
        for (int i = 0; i < propertyCount; i++) {

            QVariant
        }
        */
        /*
        int propertyCount = metaobject->propertyCount();
        for (int i = 0; i < propertyCount; i++) {
            QMetaProperty metaproperty = metaobject->property(i);
            const char *name = metaproperty.name();
            if (strcmp(name, "objectName") == 0) // not a layer property
                continue;

            QVariant::Type type = metaproperty.type();
            QWidget* widget = 0;
            if (type == QVariant::Color) {

            } else {
                QLineEdit* w = new QLineEdit();
                w->setReadOnly(true);
                //connect(w, SIGNAL(editingFinished()), editor, SLOT(updateTextProperty()));
                widget = w;
            }
            editor->registerInput(name, widget);
            widget->setProperty("variable", name);

            layout->addWidget(new QLabel(name), i, 0);
            layout->addWidget(widget, i, 1);

            QPushButton* button = new QPushButton("K");
            layout->addWidget(button, i, 2);
            button->setMaximumWidth(30);
        }
        */

        classEditors.insert(className, editor);
    }

    return classEditors[className];
}

class ImageHandler : public FileHandler
{
    QSharedPointer<Source> process(QString fileName)
    {
        if (fileName.endsWith(".jpg")) {
            return QSharedPointer<Source>(new ImageSource(fileName));
        }
        else if (fileName.endsWith(".png")) {
            return QSharedPointer<Source>(new ImageSource(fileName));
        }

        return QSharedPointer<Source>(0);
    }
};

QVector<FileHandler*> fileHandlers;

void validate()
{
    if (fileHandlers.size() == 0) {
        ImageHandler* imageHandler = new ImageHandler();

        fileHandlers.append(imageHandler);
    }
}

Mat Source::render(int frame)
{
    Mat base = renderBase(frame);
    Mat processed = _effectsList->process(base, frame);

    return processed;
}

QSharedPointer<Source> Source::getSource(QString fileName)
{
    validate();

    foreach(FileHandler* handler, fileHandlers) {
        QSharedPointer<Source> src = handler->process(fileName);
        src->_position = "[0,0]";
        src->_scale = "[1,1]";

        src->_properties.insert(QString("position"), QVariant::fromValue(KeyablePointF()));
        src->_properties.insert(QString("resolution"), QVariant::fromValue(KeyablePoint()));

        if (!src.isNull()) {
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

    if (type == qMetaTypeId<KeyablePoint>()) {

    } else if (type == qMetaTypeId<KeyablePointF>()) {
        KeyablePointF kp = _properties[propertyName].value<KeyablePointF>();
        kp.setKeyFrame(frame);
        _properties[propertyName] = QVariant::fromValue(kp);
    }
}

void Source::setProperty(QString propertyName, QVariant value)
{
    _properties[propertyName] = value;
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

QPointF Source::evalPosition()
{
    return evalPointF(position());
}

QPointF Source::evalScale()
{
    return evalPointF(scale());
}

// properties are typed strings
// can be bound to a curve, so not editable
