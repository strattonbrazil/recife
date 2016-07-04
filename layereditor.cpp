#include "layereditor.h"

#include <iostream>
#include <QPushButton>
#include <stdexcept>

#include "keyable.h"
#include "utils.h"

LayerEditor::LayerEditor(QWidget *parent, TimeContext* frameContext) : QWidget(parent), _frameContext(frameContext)
{
}

#include <iostream>

void LayerEditor::setLayer(QSharedPointer<Layer> layer)
{
    _layer = layer;

    updateAttributes();
}

void LayerEditor::updateAttributes()
{
    // put current layer values in inputs
    QMap<QString, QVariant> properties = _layer->properties();
    QMap<QString, QVariant>::iterator i;
    int row = 0;

    const int currentFrame = _frameContext->currentFrame();
    for (i = properties.begin(); i != properties.end(); ++i) {
        QString name = i.key();
        QVariant value = i.value();

        QPushButton* keyFrameButton = 0;
        if (_buttons.contains(name))
            keyFrameButton = _buttons[name];

        int type = value.userType();

        QWidget* input = _inputs[name];
        if (type == qMetaTypeId<KeyablePointF>()) {
            QLineEdit* textInput = qobject_cast<QLineEdit*>(input);
            KeyablePointF kp = value.value<KeyablePointF>();
            QPointF p = kp.eval(currentFrame);
            textInput->setText(QString("").sprintf("(%.2f,%.2f)", p.x(), p.y()));

            if (keyFrameButton)
                setButtonKeyFrame(keyFrameButton, !kp.hasKeyFrameAt(currentFrame));
        } else {
            throw std::runtime_error("setLayer: unsupported property type");
        }

        // update keyframe buttons

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

void LayerEditor::registerButton(QString name, QPushButton* button)
{
    _buttons.insert(name, button);
}

void LayerEditor::updateTextProperty(QLineEdit* lineInput)
{
    if (lineInput == 0) {
        lineInput = qobject_cast<QLineEdit*>(sender());
    }
    QString propertyName = lineInput->property("variable").toString();

    QVariant property = _layer->properties()[propertyName];
    int type = property.userType();

    const int currentFrame = _frameContext->currentFrame();

    if (type == qMetaTypeId<KeyablePoint>()) {

    } else if (type == qMetaTypeId<KeyablePointF>()) {
        QPointF newValue = stringToPointF(lineInput->text());
        KeyablePointF kp = property.value<KeyablePointF>();
        if (kp.hasKeyFrameAt(currentFrame)) {
            kp.setPointF(newValue, currentFrame);
        } else {
            kp.setPointF(newValue, currentFrame, false);
        }

        _layer->setProperty(propertyName, QVariant::fromValue(kp));
    }
}

void LayerEditor::updateFrame(int frame)
{
    updateAttributes();
}

void LayerEditor::setKeyFrame()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString propertyName = button->property("variable").toString();

    // if it has a keyframe remove it
    //button->setProperty()
    //button->icon().

    const int frame = _frameContext->currentFrame();
    if (button->property("add_keyframe").toBool()) {
        _layer->setKeyFrame(propertyName, frame);
        _frameContext->updateFrame(frame);
        setButtonKeyFrame(button, false);

    } else {
        _layer->removeKeyFrame(propertyName, frame);
        _frameContext->updateFrame(frame);
        setButtonKeyFrame(button, true);
    }
}
