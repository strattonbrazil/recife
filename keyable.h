#ifndef KEYABLE_H
#define KEYABLE_H

#include <QMetaType>
#include <QPoint>
#include <QPointF>
#include <QMap>

class KeyablePoint
{
    QPointF _p;
    QMap<int,KeyablePoint> _keys;
};
Q_DECLARE_METATYPE(KeyablePoint);


class KeyablePointF
{
    QPointF _p;
    QMap<int,KeyablePointF> _keys;
};
Q_DECLARE_METATYPE(KeyablePointF);

#endif // KEYABLE_H
