#include "compositorpane.h"
#include "ui_compositorpane.h"

#include <QMenu>
#include <QMenuBar>

CompositorPane::CompositorPane(QWidget *parent, TimeContext* frameContext) :
    QWidget(parent),
    ui(new Ui::CompositorPane)
{
    ui->setupUi(this);

    _renderView = new RenderView(0, frameContext);
    ui->renderingFrame->layout()->addWidget(_renderView);

    ui->viewportCombo->addItem("1280x720");
    ui->viewportCombo->addItem("1920x1080");
    connect(ui->viewportCombo, SIGNAL(activated(int)), this, SLOT(viewportChanged(int)));
    ui->viewportCombo->setCurrentIndex(1);
    viewportChanged(ui->viewportCombo->currentIndex());
}

CompositorPane::~CompositorPane()
{
    delete ui;
}

void CompositorPane::setModel(LayerModel *model)
{
    _renderView->setModel(model);
}

void CompositorPane::viewportChanged(int index)
{
    QString value = ui->viewportCombo->itemText(index);
    QStringList dimensions = value.split("x");
    int width = dimensions[0].toInt();
    int height = dimensions[1].toInt();

    _renderView->setViewport(width, height);
}
