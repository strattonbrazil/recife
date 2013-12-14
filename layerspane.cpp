#include "layerspane.h"
#include "ui_layerspane.h"

LayersPane::LayersPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayersPane)
{
    ui->setupUi(this);
}

LayersPane::~LayersPane()
{
    delete ui;
}

void LayersPane::setModel(LayerModel *model)
{
    ui->layersList->setModel(model);
}

QListView* LayersPane::layersList()
{
    return ui->layersList;
}

QSharedPointer<Source> LayersPane::selectedLayer()
{
    QModelIndexList indices = ui->layersList->selectionModel()->selectedRows();
    if (indices.length() == 0)
        return QSharedPointer<Source>(0);

    LayerModel* model = qobject_cast<LayerModel*>(ui->layersList->model());
    return model->layer(indices.first().row());
}

void LayersPane::updateLayer(QSharedPointer<Source> layer)
{
    LayerModel* model = qobject_cast<LayerModel*>(ui->layersList->model());
    model->updateLayer(layer);
}
