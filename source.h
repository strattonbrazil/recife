#ifndef SOURCE_H
#define SOURCE_H

#include <QSharedPointer>
#include <QVector>
#include <opencv2/core/core.hpp>

#include "effectsmodel.h"
#include "frame.h"

using namespace cv;

class FileHandler;
class LayerEditor;

class Source : public QObject
{
    Q_OBJECT

public:
    int id() { return _id; }
    virtual QString label() = 0;
    virtual Mat renderBase(int frame) = 0;
    Mat render(int frame);
    LayerEditor* editor(FrameContext* frameContext);

    static QSharedPointer<Source> getSource(QString fileName);
    EffectsModel* effectsModel();
    void addEffect(QSharedPointer<Effect> effect);

    void setProperty(QString propertyName, QVariant value);
    QMap<QString,QVariant> properties();

    bool hasKeyFrame(int frame);
    void setKeyFrame(QString propertyName, int frame);

    /*
    Q_PROPERTY(QString position READ position WRITE setPosition)
    Q_PROPERTY(QString scale READ scale WRITE setScale)

    QString position() { return _position; }
    QString scale() { return _scale; }

    QPointF evalPosition();
    QPointF evalScale();

    void setPosition(QString p) { _position = p; emitUpdate(); }
    void setScale(QString s) { _scale = s; emitUpdate(); }
    */
signals:
    void layerChanged(Source* layer);
protected:
    EffectsModel* _effectsList;
    void emitUpdate();
private:
    QMap<QString,QVariant> _properties;

    //QString _position;
    //QString _scale;
    int _id;
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

#endif // SOURCE_H
