#include "compositorpane.h"
#include "ui_compositorpane.h"

CompositorPane::CompositorPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompositorPane)
{
    ui->setupUi(this);

    _renderView = new RenderView();
    ui->renderingFrame->layout()->addWidget(_renderView);

}

CompositorPane::~CompositorPane()
{
    delete ui;
}

void CompositorPane::setModel(LayerModel *model)
{
    _renderView->setModel(model);
}
