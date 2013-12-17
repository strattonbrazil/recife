#ifndef EFFECT_H
#define EFFECT_H

#include <QString>
#include <opencv2/core/core.hpp>
#include <QWidget>

using namespace cv;

class Effect
{
public:
    enum { REQUIRE_GRAYSCALE, REQUIRE_RGB, REQUIRE_RGBA };
    //Effect();
    QString label();
    virtual Mat process(Mat in, int frame) = 0;
    virtual int requires() = 0;
    virtual QWidget* editor() = 0;
protected:
    QString _label;
};

class ColorKeyEffect : public Effect
{
public:
    ColorKeyEffect();
    Mat process(Mat in, int frame);
    int requires() { return Effect::REQUIRE_RGBA; }
    QWidget* editor();
private:
    uchar alpha(uchar r, uchar g, uchar b);
};

#endif // EFFECT_H
