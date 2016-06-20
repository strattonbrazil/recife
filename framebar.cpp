#include "framebar.h"
#include <QPainter>
#include <QMouseEvent>

#include "utils.h"

#define UPPER_HEIGHT 12
#define LOWER_HEIGHT 48

enum
{
    NO_DRAG,
    DRAG_LEFT,
    DRAG_RIGHT
};

FrameBar::FrameBar(QWidget *parent, TimeContext* timeContext) :
    QWidget(parent), _timeContext(timeContext)
{
    setMouseTracking(true);
    setMinimumHeight(UPPER_HEIGHT + LOWER_HEIGHT);
    _draggingState = NO_DRAG;

    connect(_timeContext, SIGNAL(frameChanged(int)), this, SLOT(updateFrame(int)));
}

void FrameBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // draw region widget
    //
    painter.fillRect(0, 0, width(), UPPER_HEIGHT, Qt::gray);
    {
        QColor handleColor(0, 150, 0);
        QColor hoverHandleColor(0, 255, 0);
        QColor fillColor(0, 100, 0);

        painter.fillRect(leftRegionHandle().x(), 1,
                         rightRegionHandle().x() - leftRegionHandle().x(),
                         UPPER_HEIGHT - 2,
                         fillColor);

        QColor leftColor = (_leftHighlighted && _draggingState == NO_DRAG || _draggingState == DRAG_LEFT) ?
                    hoverHandleColor : handleColor;
        QColor rightColor = (_rightHighlighted && _draggingState == NO_DRAG || _draggingState == DRAG_RIGHT) ?
                    hoverHandleColor : handleColor;
        painter.fillRect(leftRegionHandle(), leftColor);
        painter.fillRect(rightRegionHandle(), rightColor);
        ;
    }
}

void FrameBar::setLayer(QSharedPointer<Source> layer)
{
    _layer = layer;
}

#include <iostream>

void FrameBar::mouseMoveEvent(QMouseEvent *event)
{

    _leftHighlighted = leftRegionHandle().contains(event->pos());
    _rightHighlighted = rightRegionHandle().contains(event->pos());

    if (_draggingState == DRAG_LEFT) {
        int newLeftX = event->pos().x() - _handleOffset;
        _boundsLeft = std::max(0.0f, std::min(_boundsRight, newLeftX / (float)width()));
    } else if (_draggingState == DRAG_RIGHT) {
        int newRightX = event->pos().x() - _handleOffset;
        _boundsRight = std::min(1.0f, std::max(_boundsLeft, newRightX / (float)width()));
    }

    update();
}

void FrameBar::mousePressEvent(QMouseEvent *event)
{
    if (_draggingState == NO_DRAG) {
        if (leftRegionHandle().contains(event->pos())) {
            _draggingState = DRAG_LEFT;
            _handleOffset = event->pos().x() - leftRegionHandle().x();
        } else if (rightRegionHandle().contains(event->pos())) {
            _draggingState = DRAG_RIGHT;
            _handleOffset = event->pos().x() - rightRegionHandle().x();
        }
    }
}

void FrameBar::mouseReleaseEvent(QMouseEvent *event)
{
    _draggingState = NO_DRAG;
    update();
}

void FrameBar::updateFrame(int f)
{
    update();
}

QRect FrameBar::leftRegionHandle()
{
    int left = (int)(width() * _boundsLeft);
    return QRect(std::max(0, left-UPPER_HEIGHT), 0, UPPER_HEIGHT, UPPER_HEIGHT);
}

QRect FrameBar::rightRegionHandle()
{
    int right = (int)(width() * _boundsRight);
    return QRect(std::min(right, width() - UPPER_HEIGHT), 0, UPPER_HEIGHT, UPPER_HEIGHT);
}
