#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDockWidget>
#include <QListView>
#include <QFileDialog>
#include <QLabel>

#include "compositorpane.h"
#include "layerspane.h"
#include "effectspane.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // add layer list
    QDockWidget* layerListDock = new QDockWidget();
    LayersPane* layersPane = new LayersPane();
    _layerModel = new LayerModel();
    layersPane->setModel(_layerModel);
    layerListDock->setWidget(layersPane);
    layerListDock->setTitleBarWidget(new QLabel("Layers"));
    this->addDockWidget(Qt::LeftDockWidgetArea, layerListDock);

    // add layer effects list
    QDockWidget* effectListDock = new QDockWidget();
    EffectsPane* effectsPane = new EffectsPane();
    effectListDock->setWidget(effectsPane);
    effectListDock->setTitleBarWidget(new QLabel("Layer Effects"));
    this->addDockWidget(Qt::LeftDockWidgetArea, effectListDock);

    connect(ui->actionNewProject, SIGNAL(triggered(bool)), this, SLOT(newProject()));
    connect(ui->actionImport, SIGNAL(triggered(bool)), this, SLOT(importFile()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(quit()));



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
