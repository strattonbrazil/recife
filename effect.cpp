#include "effect.h"

QString Effect::label()
{
    return _label;
}

ColorKey::ColorKey()
{
    _label = "Color Key";
}

Mat ColorKey::process(Mat in)
{
    Mat out = Mat::zeros(in.size(), in.type());

    for(int i = 0; i < in.rows; i++){
        for(int j = 0; j < in.cols; j++){
            out.at<Vec4b>(i,j) = in.at<Vec4b>(i,j);
            //v[0] = 255;
            //v[1] = 0;
            //v[2] = 0;
            //v[3] = 0.5;
        }
    }

    return out;
}
