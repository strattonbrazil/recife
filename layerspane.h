#ifndef LAYERSPANE_H
#define LAYERSPANE_H

#include <QWidget>
#include <QListView>

#include "layermodel.h"

namespace Ui {
class LayersPane;
}

class LayersPane : public QWidget
{
    Q_OBJECT
    
public:
    explicit LayersPane(QWidget *parent = 0);
    ~LayersPane();
    void setModel(LayerModel* model);
    QListView* layersList();
    QSharedPointer<Layer> selectedLayer();
    void updateLayer(QSharedPointer<Layer> layer);
private:
    Ui::LayersPane *ui;
};

#endif // LAYERSPANE_H
