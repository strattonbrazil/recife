#include "renderview.h"

#include <QPaintEvent>
#include <QPainter>

RenderView::RenderView(QWidget *parent) :
    QWidget(parent), _model(0)
{
}

#include <iostream>

void RenderView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.fillRect(0, 0, width(), height(), Qt::black);

    Mat image = _model->composite(1);
    std::cout << "height: " << image.rows << std::endl;
    std::cout << "width: " << image.cols << std::endl;

    painter.end();
}

void RenderView::setModel(LayerModel *model)
{
    _model = model;
}
