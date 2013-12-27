#include "renderview.h"

#include <QPaintEvent>
#include <QPainter>
#include <QMenu>

#include "utils.h"

RenderView::RenderView(QWidget *parent) :
    QGraphicsView(parent), _model(0), _viewportOverlay(0)
{
    setScene(new QGraphicsScene());
    //this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    this->setDragMode(QGraphicsView::ScrollHandDrag);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
}

/*
void RenderView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(0, 0, width(), height(), Qt::black);

    Mat image = _model->composite(1);

    QImage img = convertMatToQImage(image);
    QPixmap pixmap = QPixmap::fromImage(img);

    //painter.drawImage(0, 0, img);
    painter.drawPixmap(0,0,200,200,pixmap);

    painter.end();
}
*/

void RenderView::drawForeground(QPainter *painter, const QRectF &rect)
{
    //QRect viewportRect = mapFromScene(_viewportOverlay->boundingRect()).boundingRect();
    QRect viewportRect = _viewportOverlay->boundingRect().toRect();
    viewportRect.setLeft(viewportRect.left()+VIEWPORT_BORDER);
    viewportRect.setRight(viewportRect.right()-VIEWPORT_BORDER);
    viewportRect.setTop(viewportRect.top()+VIEWPORT_BORDER);
    viewportRect.setBottom(viewportRect.bottom()-VIEWPORT_BORDER);

    QPen pen(Qt::green, 2, Qt::DashLine);
    painter->setPen(pen);
    painter->drawRect(viewportRect);
}

void RenderView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QRect viewportRect = _viewportOverlay->boundingRect().toRect();
    viewportRect.setLeft(viewportRect.left()+VIEWPORT_BORDER);
    viewportRect.setRight(viewportRect.right()-VIEWPORT_BORDER);
    viewportRect.setTop(viewportRect.top()+VIEWPORT_BORDER);
    viewportRect.setBottom(viewportRect.bottom()-VIEWPORT_BORDER);

    painter->fillRect(0, 0, scene()->width(), scene()->height(), Qt::black);

    QImage checkers;
    checkers.load("/home/josh.stratton/Desktop/checkers.png");
    painter->drawImage(0, 0, checkers);
}

void RenderView::setModel(LayerModel *model)
{
    _model = model;
    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateLayers(QModelIndex,QModelIndex)));
}

void RenderView::setViewport(int width, int height)
{
    if (_viewportOverlay != 0)
        scene()->removeItem(_viewportOverlay);

    const int BORDER = VIEWPORT_BORDER;
    QImage image(width+BORDER*2,height+BORDER*2, QImage::Format_ARGB32);
    image.fill(0);
    QPixmap pixmap = QPixmap::fromImage(image);

    _viewportOverlay = scene()->addPixmap(pixmap);
}

void RenderView::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = mapToGlobal(pos);
    QMenu menu;

    menu.addAction("Center Viewport");

    QAction* selectedItem = menu.exec(globalPos);
    if (selectedItem == 0)
        return;

    if (selectedItem->text() == "Center Viewport") {
        centerOn(_viewportOverlay);
    }
}

void RenderView::updateLayers(QModelIndex top, QModelIndex bottom)
{
    // TODO: only clear changed layers
    QList<QGraphicsItem*> toRemove;
    foreach (QGraphicsItem* item, scene()->items()) {
        toRemove.append(item);
    }
    foreach (QGraphicsItem* item, toRemove) {
        scene()->removeItem(item);
    }

    QPointF viewportOffset(VIEWPORT_BORDER, VIEWPORT_BORDER);
    for (int layerIndex = top.row(); layerIndex <= bottom.row(); layerIndex++) {
        QSharedPointer<Source> layer = _model->layer(layerIndex);
        Mat image = _model->composite(1, layerIndex);

        QImage img = convertMatToQImage(image);
        QPointF scale = layer->evalScale();
        const float newWidth = img.width() * scale.x();
        const float newHeight = img.height() * scale.y();
        QImage scaledImage = img.scaled(newWidth,newHeight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

        QPixmap pixmap = QPixmap::fromImage(scaledImage);

        QGraphicsItem* item = this->scene()->addPixmap(pixmap);
        item->setPos(layer->evalPosition() + viewportOffset);
    }
}
