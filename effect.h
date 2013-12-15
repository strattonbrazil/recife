#ifndef EFFECT_H
#define EFFECT_H

#include <QString>
#include <opencv2/core/core.hpp>

using namespace cv;

class Effect
{
public:
    enum { REQUIRE_GRAYSCALE, REQUIRE_RGB, REQUIRE_RGBA };
    //Effect();
    QString label();
    virtual Mat process(Mat in, int frame) = 0;
    virtual int requires() = 0;
protected:
    QString _label;
};

class ColorKeyEffect : public Effect
{
public:
    ColorKeyEffect();
    Mat process(Mat in, int frame);
    int requires() { return Effect::REQUIRE_RGBA; }
private:
    char alpha(char r, char g, char b);
};

#endif // EFFECT_H
