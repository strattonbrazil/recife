#ifndef ATTRIBUTESPANE_H
#define ATTRIBUTESPANE_H

#include <QWidget>
#include <QSet>
#include "layer.h"
#include "framebar.h"

namespace Ui {
class AttributesPane;
}

class AttributesPane : public QWidget
{
    Q_OBJECT
    
public:
    explicit AttributesPane(QWidget *parent, TimeContext* frameContext);
    ~AttributesPane();
    
    void setLayer(QSharedPointer<Layer> layer);
    void setEffect(QSharedPointer<Effect> effect);
public slots:
    void updateFrame(int frame);
private:
    Ui::AttributesPane *ui;
    QSharedPointer<Layer> _layer;
    QSharedPointer<Effect> _effect;
    LayerEditor* _currentEditor;
    QSet<QWidget*> _preservedWidgets;
    TimeContext* _timeContext;
};

#endif // ATTRIBUTESPANE_H
