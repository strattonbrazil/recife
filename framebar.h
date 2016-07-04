#ifndef FRAMEBAR_H
#define FRAMEBAR_H

#include <QWidget>

#include "layer.h"

class FrameBar : public QWidget
{
    Q_OBJECT
public:
    explicit FrameBar(QWidget *parent, TimeContext* timeContext);
    void paintEvent(QPaintEvent *event);
    void setLayer(QSharedPointer<Layer> layer);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public slots:
    void updateFrame(int f);
private:
    QString getLeftRegionTime();
    QString getRightRegionTime();
    QRect leftRegionHandle();
    QRect rightRegionHandle();

    QSharedPointer<Layer> _layer;
    TimeContext* _timeContext;

    // for rendering the region
    float _boundsLeft = 0.0f;
    float _boundsRight = 1.0f;
    int _draggingState;
    int _handleOffset;
    bool _leftHighlighted = false;
    bool _rightHighlighted = false;
};

#endif // FRAMEBAR_H
