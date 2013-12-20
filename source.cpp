#include "source.h"

#include <QStringList>
#include <QVector>
#include <stdexcept>
#include <QFile>
#include <QDir>
#include <opencv2/highgui/highgui.hpp>
#include <QMetaObject>
#include <QMetaProperty>
#include <QHash>
#include <QFormLayout>
#include <QLineEdit>

#include "utils.h"

ImageSource::ImageSource(QString fileName)
{
    // TODO: use a namebase utility function here
    _label = fileName.split("/").last();
    _imagePath = fileName;
}

QString ImageSource::label()
{
    return _label;
}

Mat ImageSource::renderBase(int frame)
{
    return cv::imread(_imagePath.toStdString(), CV_LOAD_IMAGE_UNCHANGED);
}

QHash<QString,LayerEditor*> classEditors;
\
LayerEditor* Source::editor()
{
    const QMetaObject *metaobject = metaObject();
    QString className = metaobject->className();

    if (!classEditors.contains(className)) {
        LayerEditor* editor = new LayerEditor(0);
        QFormLayout* layout = new QFormLayout();
        editor->setLayout(layout);

        int propertyCount = metaobject->propertyCount();
        for (int i = 0; i < propertyCount; i++) {
            QMetaProperty metaproperty = metaobject->property(i);
            const char *name = metaproperty.name();
            if (strcmp(name, "objectName") == 0) // not a layer property
                continue;

            QVariant::Type type = metaproperty.type();
            QWidget* widget = 0;
            if (type == QVariant::Color) {

            } else {
                QLineEdit* w = new QLineEdit();
                connect(w, SIGNAL(editingFinished()), editor, SLOT(updateTextProperty()));
                widget = w;
            }
            editor->registerInput(name, widget);
            widget->setProperty("variable", name);
            layout->addRow(name, widget);
        }

        classEditors.insert(className, editor);
    }

    return classEditors[className];
}

class ImageHandler : public FileHandler
{
    QSharedPointer<Source> process(QString fileName)
    {
        if (fileName.endsWith(".jpg")) {
            return QSharedPointer<Source>(new ImageSource(fileName));
        }
        else if (fileName.endsWith(".png")) {
            return QSharedPointer<Source>(new ImageSource(fileName));
        }

        return QSharedPointer<Source>(0);
    }
};

QVector<FileHandler*> fileHandlers;

void validate()
{
    if (fileHandlers.size() == 0) {
        ImageHandler* imageHandler = new ImageHandler();

        fileHandlers.append(imageHandler);
    }
}

Mat Source::render(int frame)
{
    Mat base = renderBase(frame);
    Mat processed = _effectsList->process(base, frame);

    return processed;
}

QSharedPointer<Source> Source::getSource(QString fileName)
{
    validate();

    foreach(FileHandler* handler, fileHandlers) {
        QSharedPointer<Source> src = handler->process(fileName);
        if (!src.isNull()) {
            src->_effectsList = new EffectsModel();
            return src;
        }
    }

    throw std::runtime_error("failed to find appropriate importer");
}

EffectsModel* Source::effectsModel()
{
    if (_effectsList == 0)
        _effectsList = new EffectsModel();
    return _effectsList;
}

void Source::addEffect(QSharedPointer<Effect> effect)
{
    _effectsList->addEffect(effect);
}

LayerEditor::LayerEditor(QWidget *parent) : QWidget(parent)
{
}

void LayerEditor::setLayer(QSharedPointer<Source> layer)
{
    _layer = layer;

    // put current layer values in inputs
    const QMetaObject *metaobject = layer->metaObject();
    QString className = metaobject->className();

    int propertyCount = metaobject->propertyCount();
    for (int i = 0; i < propertyCount; i++) {
        QMetaProperty metaproperty = metaobject->property(i);
        const char *name = metaproperty.name();
        if (strcmp(name, "objectName") == 0) // not a layer property
            continue;

        QVariant::Type type = metaproperty.type();
        QWidget* input = _inputs[QString(name)];
        if (type == QVariant::Color) {

        } else {
            QLineEdit* textInput = qobject_cast<QLineEdit*>(input);

            if (type == QVariant::Point) {
                QPoint p = layer->property(name).toPoint();
                textInput->setText(QString("").sprintf("(%i,%i)", p.x(), p.y()));
            }
            else if (type == QVariant::PointF) {
                QPointF p = layer->property(name).toPointF();
                textInput->setText(QString("").sprintf("(%.2f,%.2f)", p.x(), p.y()));
            }
            //QLineEdit* w = new QLineEdit();
            //connect(w, SIGNAL(editingFinished()), editor, SLOT(updateTextProperty()));
            //widget = w;
        }
    }
}

void LayerEditor::registerInput(QString name, QWidget* widget)
{
    _inputs.insert(name, widget);
}

#include <iostream>

void LayerEditor::updateTextProperty()
{
    char propertyName[200];
    QLineEdit* editor = qobject_cast<QLineEdit*>(sender());
    QString qpropertyName = editor->property("variable").toString();
    strcpy(propertyName, qpropertyName.toStdString().c_str());

    QMetaProperty property = findProperty(_layer.data(), propertyName);
    QVariant::Type type = property.type();
    if (type == QVariant::Point) {
        QPoint p = stringToPoint(editor->text());
        _layer->setProperty(propertyName, p);
    } else if (type == QVariant::PointF) {
        QPointF p = stringToPointF(editor->text());
        _layer->setProperty(propertyName, p);
    } else { // assume it's a string
        assert(type == QVariant::String && "LayerEditor received an unknown property type");

        std::cout << "it's a string" << std::endl;
    }
    //_layer->setProperty()
}
