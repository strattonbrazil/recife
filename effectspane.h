#ifndef EFFECTSPANE_H
#define EFFECTSPANE_H

#include <QWidget>
#include <QAbstractListModel>

namespace Ui {
class EffectsPane;
}

class EffectsPane : public QWidget
{
    Q_OBJECT
    
public:
    explicit EffectsPane(QWidget *parent = 0);
    ~EffectsPane();
    void setModel(QAbstractListModel* model);
    
private:
    Ui::EffectsPane *ui;
};

#endif // EFFECTSPANE_H
