#ifndef LAYEREDITOR_H
#define LAYEREDITOR_H

#include <QPushButton>
#include <QLineEdit>

#include "layer.h"

class LayerEditor : public QWidget
{
    Q_OBJECT

public:
    explicit LayerEditor(QWidget *parent, TimeContext* frameContext);
    void setLayer(QSharedPointer<Layer> layer);
    void registerInput(QString name, QWidget* widget);
    void registerButton(QString name, QPushButton* button);
    void updateFrame(int frame);
public slots:
    void updateTextProperty(QLineEdit* lineInput = 0);
    void setKeyFrame();
private:
    void updateAttributes();

    QSharedPointer<Layer> _layer;
    QHash<QString,QWidget*> _inputs;
    QHash<QString,QPushButton*> _buttons;
    TimeContext* _frameContext;
};

#endif // LAYEREDITOR_H
