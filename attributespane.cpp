#include "attributespane.h"
#include "ui_attributespane.h"
#include "utils.h"
#include "layereditor.h"

AttributesPane::AttributesPane(QWidget *parent, FrameContext* frameContext) :
    QWidget(parent), _frameContext(frameContext), _currentEditor(0),
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

void AttributesPane::refresh()
{
    if (_currentEditor)
        _currentEditor->setLayer(_layer);
}

void AttributesPane::setLayer(QSharedPointer<Source> layer)
{
    _layer = layer;

    QVBoxLayout* layerLayout = qobject_cast<QVBoxLayout*>(ui->layerAttrsFrame->layout());
    clearLayout(layerLayout, _preservedWidgets);

    LayerEditor* editor = _layer->editor(_frameContext);
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
