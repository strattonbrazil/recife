#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDockWidget>
#include <QListView>
#include <QFileDialog>
#include <QLabel>
#include <QSettings>
#include <iostream>

#include "compositorpane.h"
#include "effectspane.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    updateRecentImports();

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

    TimeContext* frameContext = new TimeContext();
    //connect(frameContext, SIGNAL(frameChanged(int)), this, SLOT(updateFrame(int)));

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

void MainWindow::importFile(QString fileName)
{
    // pop open a dialog
    if (fileName == "") {
        QStringList extensions = Layer::supportedExtensions();
        QStringList extensionsWildCarded;
        foreach (QString extension, extensions) {
            extensionsWildCarded.append("*." + extension);
        }

        QString extensionFilter = extensionsWildCarded.join(" ");
        std::cout << "extension filter: " << extensionFilter.toStdString() << std::endl;
        fileName = QFileDialog::getOpenFileName(this, "Choose file to import", QDir::homePath(), extensionFilter);
        if (fileName == "")
            return;
    }

    QSettings settings("recife");
    QVariant recentImports = settings.value("recent_imports");
    if (recentImports.isNull()) {
        settings.setValue("recent_imports", fileName);
    } else {
        QStringList recentFileNames = recentImports.toString().split("|");
        if (!recentFileNames.contains(fileName)) {
            // TODO: cap number of recent items
            settings.setValue("recent_imports", fileName + "|" + recentImports.toString());
        }
    }
    updateRecentImports();

    _layerModel->addSource(Layer::getSource(fileName));
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

    bool connectEffectSelection = _effectsPane->effectsList()->selectionModel() == 0;

    QSharedPointer<Layer> layer = _layerModel->layer(current.row());
    _effectsPane->setModel(layer->effectsModel());

    if (connectEffectSelection) {
        connect(_effectsPane->effectsList()->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(effectSelected(QModelIndex,QModelIndex)));
    }

    _attributesPane->setLayer(layer);
    _frameBar->setLayer(layer);
}

void MainWindow::effectSelected(const QModelIndex & current, const QModelIndex & previous)
{
    QSharedPointer<Layer> layer = _layersPane->selectedLayer();
    QSharedPointer<Effect> effect = layer->effectsModel()->effect(current.row()); // _effectsPane->selectedEffect();

    _attributesPane->setEffect(effect);
}

/*
void MainWindow::rerender(QSharedPointer<Source> layer)
{

}
*/

void MainWindow::newColorKey()
{
    QSharedPointer<Layer> layer = _layersPane->selectedLayer();

    layer->addEffect(QSharedPointer<Effect>(new ColorKeyEffect()));

    _layersPane->updateLayer(layer);
    //QModelIndex topLeft = createIndex(0, 0);
    //emit(dataChanged(topLeft, topLeft));
    //rerender(layer);
}

/*
void MainWindow::updateFrame(int frame)
{
    _attributesPane->refresh(); //Layer(_layersPane->selectedLayer());
    //set
    //std::cout << "need to change layer" << std::endl;
}
*/

void MainWindow::updateRecentImports()
{
    // TODO: remove from setting if file not found?

    QSettings settings("recife");
    QVariant recentImports = settings.value("recent_imports");
    if (recentImports.isValid()) {
        std::cout << "recent imports: " + recentImports.toString().toStdString() << std::endl;
        QStringList recentFiles = recentImports.toString().split("|");

        ui->actionNoRecentImports->setVisible(recentFiles.count() == 0);

        // remove all actions except the default one
        foreach (QObject* action, ui->menuRecentImports->children()) {
            if (action != ui->actionNoRecentImports)
                ui->menuRecentImports->removeAction(qobject_cast<QAction*>(action));
        }

        // add back recent imports
        for (int i = 0; i < recentFiles.count(); i++) {
            ui->menuRecentImports->addAction(recentFiles.at(i), this, SLOT(importRecentFile()));
        }
    }
}

void MainWindow::importRecentFile()
{
    QAction* action = qobject_cast<QAction*>(sender());
    importFile(action->text());
}
