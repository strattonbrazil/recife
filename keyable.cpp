#include "keyable.h"

QPoint KeyablePoint::eval(int frame)
{
    if (_keys.size() == 0)
        return _p;
}

bool KeyablePointF::hasKeyFrames()
{
    return _keys.size() != 0;
}

bool KeyablePointF::hasKeyFrameAt(int frame)
{
    return _keys.contains(frame);
}

void KeyablePointF::setPointF(QPointF p, int frame)
{
    if (frame == 0)
        _p = p;
    else
        _keys.insert(frame, p);
}

QPointF KeyablePointF::eval(int frame)
{
    if (_keys.size() == 0)
        return _p;
}
