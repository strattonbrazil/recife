#ifndef SOURCE_H
#define SOURCE_H

#include <QSharedPointer>
#include <QVector>
#include <opencv2/core/core.hpp>

#include "effectsmodel.h"
#include <iostream>

using namespace cv;

class FileHandler;
class LayerEditor;

class Source : public QObject
{
    Q_OBJECT

public:
    virtual QString label() = 0;
    virtual Mat renderBase(int frame) = 0;
    Mat render(int frame);
    LayerEditor* editor();

    static QSharedPointer<Source> getSource(QString fileName);
    EffectsModel* effectsModel();
    void addEffect(QSharedPointer<Effect> effect);

    Q_PROPERTY(QPointF position READ position WRITE setPosition)
    Q_PROPERTY(QPointF scale READ scale WRITE setScale)

    QPointF position() { return _position; }
    QPointF scale() { return _scale; }

    void setPosition(QPointF p) { _position = p; emitUpdate(); }
    void setScale(QPointF s) { _scale = s; emitUpdate(); }
signals:
    void layerChanged(Source* layer);
protected:
    EffectsModel* _effectsList;
    void emitUpdate();
private:
    QPointF _position;
    QPointF _scale;
};

class ImageSource : public Source
{
    Q_OBJECT

public:
    ImageSource(QString fileName);
    QString label();
    Mat renderBase(int frame);

    Q_PROPERTY(QPoint resolution READ resolution WRITE setResolution)

    QPoint resolution() { return _resolution; }
    void setResolution(QPoint r) { _resolution = r; }
private:
    QString _label;
    QString _imagePath;
    QPoint _resolution;
};



class FileHandler
{
public:
    virtual QSharedPointer<Source> process(QString fileName) = 0;
};

class LayerEditor : public QWidget
{
    Q_OBJECT

public:
    explicit LayerEditor(QWidget *parent = 0);
    void setLayer(QSharedPointer<Source> layer);
    void registerInput(QString name, QWidget* widget);
public slots:
    void updateTextProperty();
private:
    QSharedPointer<Source> _layer;
    QHash<QString,QWidget*> _inputs;
};


#endif // SOURCE_H
