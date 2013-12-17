#include "effectspane.h"
#include "ui_effectspane.h"

EffectsPane::EffectsPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EffectsPane)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->noEffectsPage);
}

EffectsPane::~EffectsPane()
{
    delete ui;
}

void EffectsPane::setModel(EffectsModel *model)
{
    // TODO: swap the card out
    ui->stackedWidget->setCurrentWidget(ui->effectsPage);

    ui->effectsList->setModel(model);
}

QListView* EffectsPane::effectsList()
{
    return ui->effectsList;
}

QSharedPointer<Effect> EffectsPane::selectedEffect()
{
    QModelIndexList indices = ui->effectsList->selectionModel()->selectedRows();
    if (indices.length() == 0)
        return QSharedPointer<Effect>(0);

    EffectsModel* model = qobject_cast<EffectsModel*>(ui->effectsList->model());
    return model->effect(indices.first().row());
}
