#ifndef KEYABLE_H
#define KEYABLE_H

#include <QMetaType>
#include <QPoint>
#include <QPointF>
#include <QMap>

class KeyablePoint
{
public:
    QPoint eval(int frame);
private:
    QPoint _p;
    QMap<int,QPoint> _keys;
};
Q_DECLARE_METATYPE(KeyablePoint);


class KeyablePointF
{
public:
    bool hasKeyFrames();
    bool hasKeyFrameAt(int frame);
    void setPointF(QPointF p, int frame=0);
    QPointF eval(int frame);
private:
    QPointF _p;
    QMap<int,QPointF> _keys;
};
Q_DECLARE_METATYPE(KeyablePointF);

#endif // KEYABLE_H
