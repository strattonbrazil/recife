#ifndef FRAME_H
#define FRAME_H

#include <QObject>

class TimeContext : public QObject
{
    Q_OBJECT
public:
    TimeContext();
    int currentFrame();
    float currentTime();
    float duration();
    void setCurrentTime(float t);
    void updateFrame(int f);
signals:
    void frameChanged(int);
private:
    float _currentTime = 0.0f;
    float _duration = 60*60*2; // 2 hours in seconds
};

#endif // FRAME_H
