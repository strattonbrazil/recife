#ifndef SOURCE_H
#define SOURCE_H

#include <QSharedPointer>
#include <QVector>
#include <opencv2/core/core.hpp>

#include "effectsmodel.h"

using namespace cv;

class FileHandler;

class Source
{
public:
    virtual QString label() = 0;
    virtual Mat renderBase(int frame) = 0;
    Mat render(int frame);
    virtual QWidget* editor() = 0;

    static QSharedPointer<Source> getSource(QString fileName);
    EffectsModel* effectsModel();
    void addEffect(QSharedPointer<Effect> effect);

protected:
    EffectsModel* _effectsList;
};

class ImageSource : public Source
{
public:
    ImageSource(QString fileName);
    QString label();
    Mat renderBase(int frame);
    QWidget* editor();
private:
    QString _label;
    QString _imagePath;
};



class FileHandler
{
public:
    virtual QSharedPointer<Source> process(QString fileName) = 0;
};

#endif // SOURCE_H
