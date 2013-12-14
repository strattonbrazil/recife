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

void EffectsPane::setModel(QAbstractListModel *model)
{
    // TODO: swap the card out
    ui->stackedWidget->setCurrentWidget(ui->effectsPage);

    ui->effectsList->setModel(model);
}
