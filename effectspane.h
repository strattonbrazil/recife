#ifndef EFFECTSPANE_H
#define EFFECTSPANE_H

#include <QWidget>
//#include <QAbstractListModel>
#include <QListView>

#include "effectsmodel.h"

namespace Ui {
class EffectsPane;
}

class EffectsPane : public QWidget
{
    Q_OBJECT
    
public:
    explicit EffectsPane(QWidget *parent = 0);
    ~EffectsPane();
    void setModel(EffectsModel* model);
    QListView* effectsList();
    QSharedPointer<Effect> selectedEffect();
private:
    Ui::EffectsPane *ui;
};

#endif // EFFECTSPANE_H
