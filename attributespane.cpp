#include "attributespane.h"
#include "ui_attributespane.h"
#include "utils.h"

AttributesPane::AttributesPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AttributesPane)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->noAttributesPage);
    ui->effectAttrsLabel->hide();
}

AttributesPane::~AttributesPane()
{
    delete ui;
}

void AttributesPane::setLayer(QSharedPointer<Source> layer)
{
    _layer = layer;

    QVBoxLayout* layerLayout = qobject_cast<QVBoxLayout*>(ui->layerAttrsFrame->layout());
    clearLayout(layerLayout);
    layerLayout->addWidget(_layer->editor());
    //layerLayout->addStretch();

    ui->stackedWidget->setCurrentWidget(ui->attributesPage);
}

void AttributesPane::setEffect(QSharedPointer<Effect> effect)
{
    _effect = effect;

    QVBoxLayout* effectLayout = qobject_cast<QVBoxLayout*>(ui->effectAttrsFrame->layout());
    clearLayout(effectLayout);
    effectLayout->addWidget(_effect->editor());
    //effectLayout->addStretch();
    ui->effectAttrsLabel->show();
}
