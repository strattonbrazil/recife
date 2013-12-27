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

    Q_PROPERTY(QString position READ position WRITE setPosition)
    Q_PROPERTY(QString scale READ scale WRITE setScale)

    QString position() { return _position; }
    QString scale() { return _scale; }

    QPointF evalPosition();
    QPointF evalScale();

    void setPosition(QString p) { _position = p; emitUpdate(); }
    void setScale(QString s) { _scale = s; emitUpdate(); }
signals:
    void layerChanged(Source* layer);
protected:
    EffectsModel* _effectsList;
    void emitUpdate();
private:
    QString _position;
    QString _scale;
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
