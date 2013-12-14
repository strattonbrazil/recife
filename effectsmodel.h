#ifndef EFFECTSMODEL_H
#define EFFECTSMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>
#include <QVector>

#include "effect.h"

class EffectsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit EffectsModel(QObject *parent = 0);
    void addEffect(QSharedPointer<Effect> effect);
    Mat process(Mat in, int frame);
    
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

signals:
    
public slots:
private:
    QVector<QSharedPointer<Effect> > _effects;
};

#endif // EFFECTSMODEL_H
