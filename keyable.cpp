#include "keyable.h"

#include "utils.h"

QPoint KeyablePoint::eval(int frame)
{
    if (_keys.size() == 0)
        return _p;
}

KeyablePointF::KeyablePointF(float x, float y)
{
    _p.setX(x);
    _p.setY(y);
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

    if (_keys.contains(frame))
        return _keys[frame];

    int previousFrame = -1;
    QPointF previousValue;
    QMap<int,QPointF>::Iterator i;
    for (i = _keys.begin(); i != _keys.end(); i++) {
        if (previousFrame == -1 && i.key() > frame) { // first frame between the last key frame and this one
            return i.value();
        } else if (frame < i.key()) { // inbetween key frames
            QPointF a = previousValue;
            QPointF b = i.value();

            float x = interpolate(frame, previousFrame, a.x(), i.key(), b.x());
            float y = interpolate(frame, previousFrame, a.y(), i.key(), b.y());
            return QPointF(x, y);
        }
        previousFrame = i.key();
        previousValue = i.value();
    }
    return previousValue; // after last key frame
}

void KeyablePointF::setKeyFrame(int frame)
{
    QPointF f = eval(frame);

    _keys.insert(frame, eval(frame));
}

void KeyablePointF::removeKeyFrame(int frame)
{
    _keys.remove(frame);
}
