#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelection>

#include "layermodel.h"
#include "layerspane.h"
#include "effectspane.h"
#include "attributespane.h"

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
    void importFile();
    void quit();

    void layerSelected(const QModelIndex & current, const QModelIndex & previous);

    void newColorKey();
private:
    void setProjectActive(bool status);
    void rerender(QSharedPointer<Source> layer);

    Ui::MainWindow *ui;
    LayersPane* _layersPane;
    LayerModel* _layerModel;
    EffectsPane* _effectsPane;
    AttributesPane* _attributesPane;
};

#endif // MAINWINDOW_H
