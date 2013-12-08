#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#include <QGraphicsView>
#include <QGraphicsItem>

#include "layermodel.h"

class RenderView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit RenderView(QWidget *parent = 0);
    //void paintEvent(QPaintEvent *event);
    void drawForeground(QPainter *painter, const QRectF &rect);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void setModel(LayerModel* model);
    void setViewport(int width, int height);
signals:
    
public slots:
    void showContextMenu(const QPoint &pos);
    void updateLayers(QModelIndex,QModelIndex);
private:
    QGraphicsItem* _viewportOverlay;
    //QGraphicsItem* _activ
    LayerModel *_model;
    const int VIEWPORT_BORDER = 5;
};

#endif // RENDERVIEW_H
