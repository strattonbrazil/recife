#ifndef FRAME_H
#define FRAME_H

#include <QObject>

class TimeContext : public QObject
{
    Q_OBJECT
public:
    TimeContext(int frame=1);
    int currentFrame();
    float duration();
    void setCurrentFrame(int f);
    void updateFrame(int f);
signals:
    void frameChanged(int);
private:
    int _currentFrame;
    float _duration = 60*60*2; // 2 hours in seconds
};

#endif // FRAME_H
