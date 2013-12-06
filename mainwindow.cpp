#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDockWidget>
#include <QListView>
#include <QFileDialog>
#include <QLabel>

#include "compositorpane.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDockWidget* layerListDock = new QDockWidget();
    QListView* layerList = new QListView();
    _layerModel = new LayerModel();
    layerList->setModel(_layerModel);
    layerListDock->setWidget(layerList);
    layerListDock->setTitleBarWidget(new QLabel("Layers"));

    this->addDockWidget(Qt::LeftDockWidgetArea, layerListDock);

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
