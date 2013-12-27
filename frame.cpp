#include "frame.h"

FrameContext::FrameContext(int frame) : _currentFrame(frame)
{
}

int FrameContext::currentFrame()
{
    return _currentFrame;
}

void FrameContext::setCurrentFrame(int f)
{
    _currentFrame = f;
    emit(frameChanged(_currentFrame));
}
