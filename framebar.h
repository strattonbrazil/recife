#ifndef FRAMEBAR_H
#define FRAMEBAR_H

#include <QWidget>

#include "source.h"

class FrameBar : public QWidget
{
    Q_OBJECT
public:
    explicit FrameBar(QWidget *parent = 0);
    int currentFrame() { return _currentFrame; }
    void paintEvent(QPaintEvent *event);
    void setLayer(QSharedPointer<Source> layer);
    void mouseReleaseEvent(QMouseEvent *event);
signals:
    void frameChanged(int);
public slots:
    
private:
    QSharedPointer<Source> _layer;
    int _currentFrame;

    static const int _BORDER = 1;
    static const int _FRAME_WIDTH = 8;
};

#endif // FRAMEBAR_H
