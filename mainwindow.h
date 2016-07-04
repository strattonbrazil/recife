#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelection>

#include "layermodel.h"
#include "layerspane.h"
#include "effectspane.h"
#include "attributespane.h"
#include "framebar.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void newProject();
    void importFile(QString fileName = "");
    void importRecentFile();
    void quit();

    void layerSelected(const QModelIndex & current, const QModelIndex & previous);
    void effectSelected(const QModelIndex & current, const QModelIndex & previous);

    void newColorKey();
    //void updateFrame(int frame);
private:
    void setProjectActive(bool status);
    void rerender(QSharedPointer<Layer> layer);
    void updateRecentImports();

    Ui::MainWindow *ui;
    LayersPane* _layersPane;
    LayerModel* _layerModel;
    EffectsPane* _effectsPane;
    AttributesPane* _attributesPane;
    FrameBar* _frameBar;
};

#endif // MAINWINDOW_H
