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
    //QRect viewportRect = mapFromScene(_viewportOverlay->boundingRect()).boundingRect();
    QRect viewportRect = _viewportOverlay->boundingRect().toRect();
    viewportRect.setLeft(viewportRect.left()+VIEWPORT_BORDER);
    viewportRect.setRight(viewportRect.right()-VIEWPORT_BORDER);
    viewportRect.setTop(viewportRect.top()+VIEWPORT_BORDER);
    viewportRect.setBottom(viewportRect.bottom()-VIEWPORT_BORDER);

    //QPen pen(Qt::red, 2, Qt::DashLine);
    //painter->setPen(pen);
    painter->fillRect(0, 0, scene()->width(), scene()->height(), Qt::black);
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
    //this->centerOn(_viewportOverlay);
    //this->fitInView(_viewportOverlay, Qt::KeepAspectRatio);
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
    for (int layer = top.row(); layer <= bottom.row(); layer++) {
        Mat image = _model->composite(1, layer);

        QImage img = convertMatToQImage(image);
        QPixmap pixmap = QPixmap::fromImage(img);

        // TODO: clear previous ones
        this->scene()->addPixmap(pixmap);
    }
    /*
    QPixmap px;
     px.load(argv[1]);
     QPainter p(&px);
     p.setPen(Qt::blue);
     p.drawLine(5,5, 40, 40);
     p.end();
     QLabel label;
     label.setPixmap(px);
     */



    //QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);

    //painter.fillRect(0, 0, width(), height(), Qt::black);

    /*
    Mat image = _model->composite(1);

    QImage img = convertMatToQImage(image);
    QPixmap pixmap = QPixmap::fromImage(img);

    this->scene()->addPixmap(pixmap);
    */

    //painter.drawImage(0, 0, img);
    //painter.drawPixmap(0,0,200,200,pixmap);

    //painter.end();

    //std::cout << "woops!" << std::endl;
}
