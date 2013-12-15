#include "effect.h"

QString Effect::label()
{
    return _label;
}

ColorKeyEffect::ColorKeyEffect()
{
    _label = "Color Key";
}
#include <iostream>
Mat ColorKeyEffect::process(Mat in, int frame)
{
    Mat out = Mat::zeros(in.size(), in.type());

    std::cout << in.channels() << std::endl;

    for(int i = 0; i < in.rows; i++) {
        for(int j = 0; j < in.cols; j++){
            // TODO: check whether input frame has alpha?

            out.at<Vec4b>(i,j) = in.at<Vec4b>(i,j);
            //v[0] = 255;
            //v[1] = 0;
            //v[2] = 0;
            //v[3] = 0.5;
        }
    }

    return out;
}



