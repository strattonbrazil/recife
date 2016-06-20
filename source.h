#ifndef SOURCE_H
#define SOURCE_H

#include <QSharedPointer>
#include <QVector>
#include <opencv2/core/core.hpp>

#include "effectsmodel.h"
#include "frame.h"

using namespace cv;

//class FileHandler;
class LayerEditor;

class Source : public QObject
{
    Q_OBJECT

public:
    int id() { return _id; }
    QString label() { return _label; }
    virtual Mat renderBase(int frame) = 0;
    Mat render(int frame);
    LayerEditor* editor(TimeContext* frameContext);

    static QStringList supportedExtensions();
    static QSharedPointer<Source> getSource(QString fileName);
    EffectsModel* effectsModel();
    void addEffect(QSharedPointer<Effect> effect);

    void setProperty(QString propertyName, QVariant value);
    QMap<QString,QVariant> properties();

    bool hasKeyFrame(int frame);
    void setKeyFrame(QString propertyName, int frame);
    void removeKeyFrame(QString propertyName, int frame);
signals:
    void layerChanged(Source* layer);
protected:
    EffectsModel* _effectsList;
    void emitUpdate();

    QString _label;
private:
    QMap<QString,QVariant> _properties;

    int _id;
};

class ImageSource : public Source
{
    Q_OBJECT

public:
    ImageSource(QString fileName);
    Mat renderBase(int frame);
private:
    QString _imagePath;
    QPoint _resolution;
};

class VideoSource : public Source
{
    Q_OBJECT

public:
    VideoSource(QString fileName);
    Mat renderBase(int frame);
private:
    QString _videoPath;
};



#endif // SOURCE_H
