#ifndef ATTRIBUTESPANE_H
#define ATTRIBUTESPANE_H

#include <QWidget>
#include <QSet>
#include "source.h"

namespace Ui {
class AttributesPane;
}

class AttributesPane : public QWidget
{
    Q_OBJECT
    
public:
    explicit AttributesPane(QWidget *parent = 0);
    ~AttributesPane();
    
    void setLayer(QSharedPointer<Source> layer);
    void setEffect(QSharedPointer<Effect> effect);
private:
    Ui::AttributesPane *ui;
    QSharedPointer<Source> _layer;
    QSharedPointer<Effect> _effect;
    QSet<QWidget*> _preservedWidgets;
};

#endif // ATTRIBUTESPANE_H
