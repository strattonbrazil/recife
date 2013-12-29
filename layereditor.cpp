#include "layereditor.h"

#include <QLineEdit>
#include <QPushButton>

#include "keyable.h"
#include "utils.h"

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

    std::cout << "updated property: " << propertyName.toStdString() << " - " << editor->text().toStdString() << std::endl;
    QVariant property = _layer->properties()[propertyName];
    int type = property.userType();

    const int currentFrame = _frameContext->currentFrame();

    if (type == qMetaTypeId<KeyablePoint>()) {

    } else if (type == qMetaTypeId<KeyablePointF>()) {
        QPointF newValue = stringToPointF(editor->text());
        KeyablePointF kp = property.value<KeyablePointF>();
        if (!kp.hasKeyFrames()) {
            kp.setPointF(newValue);
        } else if (kp.hasKeyFrameAt(currentFrame)) {
            kp.setPointF(newValue, currentFrame);
        }


        _layer->setProperty(propertyName, QVariant::fromValue(kp));
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

void LayerEditor::setKeyFrame()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString propertyName = button->property("variable").toString();

    const int frame = _frameContext->currentFrame();
    _layer->setKeyFrame(propertyName, frame);
    _frameContext->updateFrame(frame);
}
