#include "effectspane.h"
#include "ui_effectspane.h"

EffectsPane::EffectsPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EffectsPane)
{
    ui->setupUi(this);
}

EffectsPane::~EffectsPane()
{
    delete ui;
}
