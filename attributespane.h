#ifndef ATTRIBUTESPANE_H
#define ATTRIBUTESPANE_H

#include <QWidget>
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
private:
    Ui::AttributesPane *ui;
    QSharedPointer<Source> _layer;
};

#endif // ATTRIBUTESPANE_H
