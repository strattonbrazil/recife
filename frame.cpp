#include "frame.h"

TimeContext::TimeContext(int frame) : _currentFrame(frame)
{
}

int TimeContext::currentFrame()
{
    return _currentFrame;
}

float TimeContext::duration()
{
    return _duration;
}

void TimeContext::setCurrentFrame(int f)
{
    _currentFrame = f;
    emit(frameChanged(_currentFrame));
}

void TimeContext::updateFrame(int f)
{
    emit(frameChanged(_currentFrame));
}
