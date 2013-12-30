#ifndef LAYEREDITOR_H
#define LAYEREDITOR_H

#include <QPushButton>

#include "source.h"

class LayerEditor : public QWidget
{
    Q_OBJECT

public:
    explicit LayerEditor(QWidget *parent, FrameContext* frameContext);
    void setLayer(QSharedPointer<Source> layer);
    void registerInput(QString name, QWidget* widget);
    void registerButton(QString name, QPushButton* button);
public slots:
    void updateTextProperty();
    void setKeyFrame();
private:
    QSharedPointer<Source> _layer;
    QHash<QString,QWidget*> _inputs;
    QHash<QString,QPushButton*> _buttons;
    FrameContext* _frameContext;
};

#endif // LAYEREDITOR_H
