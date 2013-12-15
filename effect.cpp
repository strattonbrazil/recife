#include "effect.h"

QString Effect::label()
{
    return _label;
}

ColorKeyEffect::ColorKeyEffect()
{
    _label = "Color Key";
}

Mat ColorKeyEffect::process(Mat in, int frame)
{
    Mat out = Mat::zeros(in.size(), in.type());

    for(int i = 0; i < in.rows; i++) {
        for(int j = 0; j < in.cols; j++){
            // TODO: check whether input frame has alpha?

            Vec4b rgba = in.at<Vec4b>(i,j);
            rgba[3] = alpha(rgba[0], rgba[1], rgba[2]);
            rgba[2] = 0;
            out.at<Vec4b>(i,j) = rgba;
        }
    }

    return out;
}

char ColorKeyEffect::alpha(char r, char g, char b)
{
    return 128;
}



