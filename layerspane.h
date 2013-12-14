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
    QSharedPointer<Source> selectedLayer();
private:
    Ui::LayersPane *ui;
};

#endif // LAYERSPANE_H
