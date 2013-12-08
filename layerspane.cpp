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
