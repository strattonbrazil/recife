#ifndef ATTRIBUTESPANE_H
#define ATTRIBUTESPANE_H

#include <QWidget>
#include <QSet>
#include "source.h"
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
    
    void refresh();
    void setLayer(QSharedPointer<Source> layer);
    void setEffect(QSharedPointer<Effect> effect);
private:
    Ui::AttributesPane *ui;
    QSharedPointer<Source> _layer;
    QSharedPointer<Effect> _effect;
    LayerEditor* _currentEditor;
    QSet<QWidget*> _preservedWidgets;
    TimeContext* _frameContext;
};

#endif // ATTRIBUTESPANE_H
