#ifndef LAYERMODEL_H
#define LAYERMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>

#include "layer.h"

using namespace cv;

class LayerModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit LayerModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    void addSource(QSharedPointer<Layer> source);
    Mat composite(int frame, int layer);
    QSharedPointer<Layer> layer(int layer);
    void updateLayer(QSharedPointer<Layer> layer);

public slots:
    void layerChanged(Layer* layer);
private:
    QVector<QSharedPointer<Layer> > _sources;
    
};

#endif // LAYERMODEL_H
