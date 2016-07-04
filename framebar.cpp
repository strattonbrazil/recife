#include "framebar.h"
#include <QPainter>
#include <QMouseEvent>
#include <QTime>

#include "utils.h"

#define UPPER_HEIGHT 12
#define LOWER_HEIGHT 48

enum
{
    NO_DRAG,
    DRAG_LEFT,
    DRAG_RIGHT,
    DRAG_FRAME
};

FrameBar::FrameBar(QWidget *parent, TimeContext* timeContext) :
    QWidget(parent), _timeContext(timeContext)
{
    setMouseTracking(true);
    setMinimumHeight(UPPER_HEIGHT + LOWER_HEIGHT);
    _draggingState = NO_DRAG;

    connect(_timeContext, SIGNAL(frameChanged(int)), this, SLOT(updateFrame(int)));
}

QString FrameBar::getLeftRegionTime()
{
    int totalSeconds = 60 * 60 * 2; // hard-code for now as 2 hours
    int secondsOnLeft = totalSeconds * _boundsLeft;
    int minutes = secondsOnLeft / 60;
    int seconds = secondsOnLeft - (minutes * 60);

    return QString("%1:%2").arg(minutes, 2, 10, QChar('0'))
                           .arg(seconds, 2, 10, QChar('0'));
}

QString FrameBar::getRightRegionTime()
{
    int totalSeconds = _timeContext->duration(); // hard-code for now as 2 hours
    int secondsOnRight = totalSeconds * _boundsRight;
    int minutes = secondsOnRight / 60;
    int seconds = secondsOnRight - (minutes * 60);

    return QString("%1:%2").arg(minutes, 2, 10, QChar('0'))
                           .arg(seconds, 2, 10, QChar('0'));
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
    }

    // draw the timeline widget
    //
    {
        QColor timelineColor(0, 160, 0);
        painter.fillRect(0, UPPER_HEIGHT, width(), LOWER_HEIGHT, timelineColor);

        painter.drawText(10, UPPER_HEIGHT, getLeftRegionTime());

        float seconds = _timeContext->currentTime();
        float secondsLeft = _timeContext->duration() * _boundsLeft;
        float secondsRight = _timeContext->duration() * _boundsRight;
        const int markerX = ((seconds - secondsLeft) / (secondsRight - secondsLeft)) * width();
        painter.drawLine(markerX, UPPER_HEIGHT, markerX, height());

        QString rightRegionTime = getRightRegionTime();
        int strWidth = painter.fontMetrics().width(rightRegionTime);
        painter.drawText(width() - strWidth, UPPER_HEIGHT, rightRegionTime);
    }

}

void FrameBar::setLayer(QSharedPointer<Source> layer)
{
    _layer = layer;
}

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
        } else if (event->pos().y() > UPPER_HEIGHT) {
            float secondsLeft = _timeContext->duration() * _boundsLeft;
            float secondsRight = _timeContext->duration() * _boundsRight;
            float pickSeconds = secondsLeft + (secondsRight - secondsLeft) * (event->x() / (float)width());
            _timeContext->setCurrentTime(pickSeconds);
            _draggingState = DRAG_FRAME;
        }
    }

    update();
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
