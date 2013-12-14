#ifndef LAYERMODEL_H
#define LAYERMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>

#include "source.h"

using namespace cv;

class LayerModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit LayerModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    void addSource(QSharedPointer<Source> source);
    Mat composite(int frame, int layer);
    QSharedPointer<Source> layer(int layer);
signals:

public slots:
private:
    QVector<QSharedPointer<Source> > _sources;
    
};

#endif // LAYERMODEL_H
