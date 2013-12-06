#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#include <QWidget>

#include "layermodel.h"

class RenderView : public QWidget
{
    Q_OBJECT
public:
    explicit RenderView(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void setModel(LayerModel* model);
signals:
    
public slots:
    
private:
    LayerModel *_model;
};

#endif // RENDERVIEW_H
