#include "frame.h"

TimeContext::TimeContext()
{
}

int TimeContext::currentFrame()
{
    // for-now assume 24 frames per second
    return 24 * _currentTime + 1;
}

float TimeContext::currentTime()
{
    return _currentTime;
}

void TimeContext::setCurrentTime(float t)
{
    _currentTime = t;
    updateFrame(5);
}

float TimeContext::duration()
{
    return _duration;
}

void TimeContext::updateFrame(int f)
{
    emit(frameChanged(currentFrame()));
}
