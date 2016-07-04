#include "attributespane.h"
#include "ui_attributespane.h"
#include "utils.h"
#include "layereditor.h"

AttributesPane::AttributesPane(QWidget *parent, TimeContext* frameContext) :
    QWidget(parent), _timeContext(frameContext), _currentEditor(0),
    ui(new Ui::AttributesPane)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->noAttributesPage);
    ui->effectAttrsLabel->hide();

    connect(_timeContext, SIGNAL(frameChanged(int)), this, SLOT(updateFrame(int)));
}

AttributesPane::~AttributesPane()
{
    delete ui;
}

void AttributesPane::setLayer(QSharedPointer<Layer> layer)
{
    _layer = layer;

    QVBoxLayout* layerLayout = qobject_cast<QVBoxLayout*>(ui->layerAttrsFrame->layout());
    clearLayout(layerLayout, _preservedWidgets);

    LayerEditor* editor = _layer->editor(_timeContext);
    layerLayout->addWidget(editor);
    _preservedWidgets.insert(editor);

    editor->setLayer(_layer);
    _currentEditor = editor;

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

void AttributesPane::updateFrame(int frame)
{
    _currentEditor->updateFrame(frame);
}
