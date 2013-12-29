#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDockWidget>
#include <QListView>
#include <QFileDialog>
#include <QLabel>

#include "compositorpane.h"
#include "effectspane.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // add layer list
    QDockWidget* layerListDock = new QDockWidget();
    _layersPane = new LayersPane();
    _layerModel = new LayerModel();
    _layersPane->setModel(_layerModel);
    layerListDock->setWidget(_layersPane);
    layerListDock->setTitleBarWidget(new QLabel("Layers"));
    this->addDockWidget(Qt::LeftDockWidgetArea, layerListDock);

    connect(_layersPane->layersList()->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(layerSelected(QModelIndex,QModelIndex)));

    // add layer effects list
    QDockWidget* effectListDock = new QDockWidget();
    _effectsPane = new EffectsPane();
    effectListDock->setWidget(_effectsPane);
    effectListDock->setTitleBarWidget(new QLabel("Layer Effects"));
    this->addDockWidget(Qt::LeftDockWidgetArea, effectListDock);

    // connect menu options
    //
    connect(ui->actionNewProject, SIGNAL(triggered(bool)), this, SLOT(newProject()));
    connect(ui->actionImport, SIGNAL(triggered(bool)), this, SLOT(importFile()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(quit()));

    connect(ui->actionColorKeyEffect, SIGNAL(triggered(bool)), this, SLOT(newColorKey()));

    FrameContext* frameContext = new FrameContext();
    connect(frameContext, SIGNAL(frameChanged(int)), this, SLOT(updateFrame(int)));

    // add attributes pane
    QDockWidget* attributeListDock = new QDockWidget();
    _attributesPane = new AttributesPane(0, frameContext);
    attributeListDock->setWidget(_attributesPane);
    attributeListDock->setTitleBarWidget(new QLabel("Attributes"));
    this->addDockWidget(Qt::RightDockWidgetArea, attributeListDock);

    // add frame bar
    _frameBar = new FrameBar(0, frameContext);
    ui->framesFrame->layout()->addWidget(_frameBar);

    // make a new tab
    CompositorPane* renderPane = new CompositorPane(0, frameContext);
    renderPane->setModel(_layerModel);
    QVBoxLayout* layout = new QVBoxLayout();
    ui->renderTabs->widget(0)->setLayout(layout);
    layout->addWidget(renderPane);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::importFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Choose file to import", QDir::homePath(), "*.mpg *.jpg *.png");
    if (fileName == "")
        return;

    _layerModel->addSource(Source::getSource(fileName));
}

void MainWindow::newProject()
{
    setProjectActive(true);
}

void MainWindow::quit()
{

}

void MainWindow::setProjectActive(bool status)
{
}

#include <iostream>


void MainWindow::layerSelected(const QModelIndex & current, const QModelIndex & previous)
{
    ui->actionColorKeyEffect->setEnabled(true);

    bool connectEffectSelection = _effectsPane->effectsList()->selectionModel() == 0;

    QSharedPointer<Source> layer = _layerModel->layer(current.row());
    _effectsPane->setModel(layer->effectsModel());

    if (connectEffectSelection) {
        connect(_effectsPane->effectsList()->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(effectSelected(QModelIndex,QModelIndex)));
    }

    _attributesPane->setLayer(layer);
    _frameBar->setLayer(layer);
}

void MainWindow::effectSelected(const QModelIndex & current, const QModelIndex & previous)
{
    QSharedPointer<Source> layer = _layersPane->selectedLayer();
    QSharedPointer<Effect> effect = layer->effectsModel()->effect(current.row()); // _effectsPane->selectedEffect();

    _attributesPane->setEffect(effect);

    std::cout << "selected effect" << std::endl;
}

/*
void MainWindow::rerender(QSharedPointer<Source> layer)
{

}
*/

void MainWindow::newColorKey()
{
    QSharedPointer<Source> layer = _layersPane->selectedLayer();

    layer->addEffect(QSharedPointer<Effect>(new ColorKeyEffect()));

    _layersPane->updateLayer(layer);
    //QModelIndex topLeft = createIndex(0, 0);
    //emit(dataChanged(topLeft, topLeft));
    //rerender(layer);
}

void MainWindow::updateFrame(int frame)
{
    _attributesPane->refresh(); //Layer(_layersPane->selectedLayer());
    //set
    //std::cout << "need to change layer" << std::endl;
}
