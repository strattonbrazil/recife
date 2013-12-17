#include "effect.h"
#include <QColor>

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
            rgba.val[3] = saturate_cast<uchar>(alpha(rgba[0], rgba[1], rgba[2]));
            out.at<Vec4b>(i,j) = rgba;
        }
    }

    return out;
}

uchar ColorKeyEffect::alpha(uchar r, uchar g, uchar b)
{
    qreal h, s, v;
    //std::cout << (int)r << " " << (int)g << " " << (int)b << std::endl;

    uchar foo = max(r,b);
    if ((float)g / foo > 2) {
        return 0;
    }
    //QColor color(r, g, b);
    //color.getHsvF(&h, &s, &v);

    return 255;
}

#include <QLabel>

QWidget* ColorKeyEffect::editor()
{
    return new QLabel("this is another!");
}


