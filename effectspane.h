#ifndef EFFECTSPANE_H
#define EFFECTSPANE_H

#include <QWidget>

namespace Ui {
class EffectsPane;
}

class EffectsPane : public QWidget
{
    Q_OBJECT
    
public:
    explicit EffectsPane(QWidget *parent = 0);
    ~EffectsPane();
    
private:
    Ui::EffectsPane *ui;
};

#endif // EFFECTSPANE_H
