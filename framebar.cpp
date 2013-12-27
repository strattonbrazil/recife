#include "framebar.h"
#include <QPainter>
#include <QMouseEvent>

#include "utils.h"

FrameBar::FrameBar(QWidget *parent) :
    QWidget(parent), _currentFrame(1)
{
    setMinimumHeight(40);
}

void FrameBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.fillRect(0, 0, width(), height(), Qt::black);

    const int BORDER = _BORDER;
    const int FRAME_WIDTH = _FRAME_WIDTH;
    const int FRAME_HEIGHT = height() - (2*BORDER);

    QColor colors[] = { Qt::white, Qt::gray };

    for (int frame = 1; frame <= 240; frame++) {
        int x = (frame-1)*FRAME_WIDTH+BORDER;
        painter.fillRect(x, BORDER, FRAME_WIDTH, FRAME_HEIGHT, colors[frame%2]);

        if (_layer->hasKeyFrame(frame)) {
            const int ICON_WIDTH = FRAME_WIDTH - 2;
            painter.fillRect(x + 1, BORDER + 2, ICON_WIDTH, ICON_WIDTH, Qt::black);
        }
    }



    painter.setPen(QColor(255,0,255));
    for (int i = 0; i < BORDER; i++)
        painter.drawRect((_currentFrame-1)*FRAME_WIDTH+i, i, FRAME_WIDTH+BORDER+BORDER-1-i*2, height()-1-i*2);
    painter.fillRect((_currentFrame-1)*FRAME_WIDTH+BORDER, BORDER, FRAME_WIDTH, FRAME_HEIGHT, QColor(255,0,255,60));

    painter.end();
}

void FrameBar::setLayer(QSharedPointer<Source> layer)
{
    _layer = layer;
}

void FrameBar::mouseReleaseEvent(QMouseEvent *event)
{
    int frame = 1 + (event->pos().x()-_BORDER) / _FRAME_WIDTH;

    if (frame != _currentFrame) {
        //std::cout << "frame: " << frame << std::endl;
        _currentFrame = frame;
        update();
        emit(frameChanged(_currentFrame));
    }

}
