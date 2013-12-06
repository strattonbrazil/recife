#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "layermodel.h"

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
private:
    void setProjectActive(bool status);

    Ui::MainWindow *ui;
    LayerModel* _layerModel;
};

#endif // MAINWINDOW_H
