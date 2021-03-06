#include "layermodel.h"

LayerModel::LayerModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int LayerModel::rowCount(const QModelIndex &parent) const
{
    return _sources.size();
}

QVariant LayerModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    return QVariant(_sources[index.row()]->label());
}

void LayerModel::addSource(QSharedPointer<Layer> source)
{
    _sources.append(source);

    connect(source.data(), SIGNAL(layerChanged(Layer*)), this, SLOT(layerChanged(Layer*)));

    QModelIndex topLeft = createIndex(0, 0);
    emit(dataChanged(topLeft, topLeft));
}

Mat LayerModel::composite(int frame, int layer)
{
    return _sources[layer]->render(frame);
}

QSharedPointer<Layer> LayerModel::layer(int layer)
{
    return _sources[layer];
}

void LayerModel::updateLayer(QSharedPointer<Layer> layer)
{
    // TODO: actually find the right model index instead of sending
    // all of them
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomLeft = createIndex(_sources.size()-1, 0);

    emit(dataChanged(topLeft, bottomLeft));
}

void LayerModel::layerChanged(Layer *layer)
{
    // TODO: actually find the right model index instead of sending
    // all of them
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomLeft = createIndex(_sources.size()-1, 0);

    emit(dataChanged(topLeft, bottomLeft));
}
