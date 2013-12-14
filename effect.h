#ifndef EFFECT_H
#define EFFECT_H

#include <QString>
#include <opencv2/core/core.hpp>

using namespace cv;

class Effect
{
public:
    //Effect();
    virtual QString label();
    virtual Mat process(Mat in) = 0;
protected:
    QString _label;
};

class ColorKey : public Effect
{
public:
    ColorKey();
    Mat process(Mat in);
};

#endif // EFFECT_H
