#ifndef LAYEREDITOR_H
#define LAYEREDITOR_H

#include "source.h"

class LayerEditor : public QWidget
{
    Q_OBJECT

public:
    explicit LayerEditor(QWidget *parent, FrameContext* frameContext);
    void setLayer(QSharedPointer<Source> layer);
    void registerInput(QString name, QWidget* widget);
public slots:
    void updateTextProperty();
    void setKeyFrame();
private:
    QSharedPointer<Source> _layer;
    QHash<QString,QWidget*> _inputs;
    FrameContext* _frameContext;
};

#endif // LAYEREDITOR_H
