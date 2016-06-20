#ifndef FRAMEBAR_H
#define FRAMEBAR_H

#include <QWidget>

#include "source.h"

class FrameBar : public QWidget
{
    Q_OBJECT
public:
    explicit FrameBar(QWidget *parent, TimeContext* timeContext);
    void paintEvent(QPaintEvent *event);
    void setLayer(QSharedPointer<Source> layer);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public slots:
    void updateFrame(int f);
private:
    QRect leftRegionHandle();
    QRect rightRegionHandle();

    QSharedPointer<Source> _layer;
    TimeContext* _timeContext;

    // for rendering the region
    float _boundsLeft = 0.2f;
    float _boundsRight = 0.8f;
    int _draggingState;
    int _handleOffset;
    bool _leftHighlighted = false;
    bool _rightHighlighted = false;
};

#endif // FRAMEBAR_H
