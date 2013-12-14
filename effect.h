#ifndef EFFECT_H
#define EFFECT_H

#include <QString>
#include <opencv2/core/core.hpp>

using namespace cv;

class Effect
{
public:
    //Effect();
    QString label();
    virtual Mat process(Mat in, int frame) = 0;
protected:
    QString _label;
};

class ColorKeyEffect : public Effect
{
public:
    ColorKeyEffect();
    Mat process(Mat in, int frame);
};

#endif // EFFECT_H
