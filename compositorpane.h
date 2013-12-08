#ifndef COMPOSITORPANE_H
#define COMPOSITORPANE_H

#include <QWidget>

#include "layermodel.h"
#include "renderview.h"

namespace Ui {
class CompositorPane;
}

class CompositorPane : public QWidget
{
    Q_OBJECT
    
public:
    explicit CompositorPane(QWidget *parent = 0);
    ~CompositorPane();
    
    void setModel(LayerModel* model);
public slots:
    void viewportChanged(int index);
private:
    Ui::CompositorPane *ui;
    RenderView* _renderView;
};

#endif // COMPOSITORPANE_H
