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

void LayerModel::addSource(QSharedPointer<Source> source)
{
    _sources.append(source);

    QModelIndex topLeft = createIndex(0, 0);
    emit(dataChanged(topLeft, topLeft));
}

Mat LayerModel::composite(int frame)
{
    char *file = "/home/josh.stratton/Pictures/hulu.png";
    return cv::imread(file, CV_LOAD_IMAGE_COLOR);
}
