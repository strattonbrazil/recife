#include "attributespane.h"
#include "ui_attributespane.h"

AttributesPane::AttributesPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AttributesPane)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->noAttributesPage);

}

AttributesPane::~AttributesPane()
{
    delete ui;
}

void AttributesPane::setLayer(QSharedPointer<Source> layer)
{
    _layer = layer;

    ui->stackedWidget->setCurrentWidget(ui->attributesPage);
}
