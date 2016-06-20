#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#include <QGraphicsView>
#include <QGraphicsItem>

#include "layermodel.h"

class RenderView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit RenderView(QWidget *parent, TimeContext* frameContext);
    //void paintEvent(QPaintEvent *event);
    void drawForeground(QPainter *painter, const QRectF &rect);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void setModel(LayerModel* model);
    void setViewport(int width, int height);
signals:
    
public slots:
    void showContextMenu(const QPoint &pos);
    void updateLayers(QModelIndex,QModelIndex);
    void updateFrame(int frame);
private:
    QGraphicsItem* _viewportOverlay;
    //QGraphicsItem* _activ
    LayerModel *_model;
    TimeContext* _frameContext;
    QHash<int, QGraphicsItem*> _layerIdToItem;
    const int VIEWPORT_BORDER = 5;
};

#endif // RENDERVIEW_H
