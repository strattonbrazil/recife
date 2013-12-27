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

LayerEditor::LayerEditor(QWidget *parent, FrameContext* frameContext) : QWidget(parent), _frameContext(frameContext)
{
}

void LayerEditor::setLayer(QSharedPointer<Source> layer)
{
    _layer = layer;

    // put current layer values in inputs
    QMap<QString, QVariant> properties = layer->properties();
    QMap<QString, QVariant>::iterator i;
    int row = 0;

    const int currentFrame = _frameContext->currentFrame();
    for (i = properties.begin(); i != properties.end(); ++i) {
        QString name = i.key();
        QVariant value = i.value();

        int type = value.userType();

        QWidget* input = _inputs[QString(name)];
        if (type == qMetaTypeId<KeyablePoint>()) {
            QLineEdit* textInput = qobject_cast<QLineEdit*>(input);
            KeyablePoint p = value.value<KeyablePoint>();

            //QVariant::convert( value.to
            //std::cout << "it's a point" << std::endl;
        } else if (type == qMetaTypeId<KeyablePointF>()) {
            QLineEdit* textInput = qobject_cast<QLineEdit*>(input);
            KeyablePointF kp = value.value<KeyablePointF>();
            QPointF p = kp.eval(currentFrame);
            textInput->setText(QString("").sprintf("(%.2f,%.2f)", p.x(), p.y()));
        }
    }
    /*
    const QMetaObject *metaobject = layer->metaObject();
    QString className = metaobject->className();

    int propertyCount = metaobject->propertyCount();
    for (int i = 0; i < propertyCount; i++) {
        QMetaProperty metaproperty = metaobject->property(i);
        const char *name = metaproperty.name();
        if (strcmp(name, "objectName") == 0) // not a layer property
            continue;

        QVariant::Type type = metaproperty.type();
        QWidget* input = _inputs[QString(name)];
        if (type == QVariant::Color) {

        } else {
            QLineEdit* textInput = qobject_cast<QLineEdit*>(input);

            if (type == QVariant::Point) {
                QPoint p = layer->property(name).toPoint();
                textInput->setText(QString("").sprintf("(%i,%i)", p.x(), p.y()));
            }
            else if (type == QVariant::PointF) {
                QPointF p = layer->property(name).toPointF();
                textInput->setText(QString("").sprintf("(%.2f,%.2f)", p.x(), p.y()));
            }
            else if (type == QVariant::String) {
                QString s = layer->property(name).toString();
                textInput->setText(s);
            }
        }
    }
    */
}

void LayerEditor::registerInput(QString name, QWidget* widget)
{
    _inputs.insert(name, widget);
}

#include <iostream>

void LayerEditor::updateTextProperty()
{
    QLineEdit* editor = qobject_cast<QLineEdit*>(sender());
    QString propertyName = editor->property("variable").toString();

    std::cout << "updated property: " << propertyName.toStdString() << std::endl;
    QVariant property = _layer->properties()[propertyName];
    int type = property.userType();

    const int currentFrame = _frameContext->currentFrame();

    if (type == qMetaTypeId<KeyablePoint>()) {

    } else if (type == qMetaTypeId<KeyablePointF>()) {
        QPointF newValue(9,9);
        KeyablePointF kp = property.value<KeyablePointF>();
        if (!kp.hasKeyFrames()) {
            kp.setPointF(newValue);
        } else if (kp.hasKeyFrameAt(currentFrame)) {
            kp.setPointF(newValue, currentFrame);
        }
    }


    // find text input
    // update property
    // go to
    /*
    char propertyName[200];

    strcpy(propertyName, qpropertyName.toStdString().c_str());

    QMetaProperty property = findProperty(_layer.data(), propertyName);
    QVariant::Type type = property.type();
    if (type == QVariant::Point) {
        QPoint p = stringToPoint(editor->text());
        _layer->setProperty(propertyName, p);
    } else if (type == QVariant::PointF) {
        QPointF p = stringToPointF(editor->text());
        _layer->setProperty(propertyName, p);
    } else { // assume it's a string
        assert(type == QVariant::String && "LayerEditor received an unknown property type");

        QString text = editor->text();
        evalPointF(text);

        _layer->setProperty(propertyName, editor->text());
    }

    //_layer->setProperty()
    */
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
