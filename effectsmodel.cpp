#include "effectsmodel.h"

EffectsModel::EffectsModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int EffectsModel::rowCount(const QModelIndex &parent) const
{
    return _effects.size();
}

QVariant EffectsModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    return _effects[index.row()]->label();
}

void EffectsModel::addEffect(QSharedPointer<Effect> effect)
{
    _effects.append(effect);

    QModelIndex topLeft = createIndex(0, 0);
    emit(dataChanged(topLeft, topLeft));
}
