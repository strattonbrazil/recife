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

    connect(ui->actionNewProject, SIGNAL(triggered(bool)), this, SLOT(newProject()));
    connect(ui->actionImport, SIGNAL(triggered(bool)), this, SLOT(importFile()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(quit()));

    connect(ui->actionColorKeyEffect, SIGNAL(triggered(bool)), this, SLOT(newColorKey()));

    // make a new tab
    CompositorPane* renderPane = new CompositorPane();
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

void MainWindow::layerSelected(const QModelIndex & current, const QModelIndex & previous)
{
    ui->actionColorKeyEffect->setEnabled(true);

    QSharedPointer<Source> layer = _layerModel->layer(current.row());
    _effectsPane->setModel(layer->effectsModel());
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
