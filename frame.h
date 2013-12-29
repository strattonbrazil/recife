#ifndef FRAME_H
#define FRAME_H

#include <QObject>

class FrameContext : public QObject
{
    Q_OBJECT
public:
    FrameContext(int frame=1);
    int currentFrame();
    void setCurrentFrame(int f);
    void updateFrame(int f);
signals:
    void frameChanged(int);
private:
    int _currentFrame;
};

#endif // FRAME_H
