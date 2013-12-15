#include "utils.h"

#include <opencv2/ocl/ocl.hpp>

#include <iostream>

QImage convertMatToQImage(cv::Mat const& src)
{
    // make sure there's an alpha channel
    cv::Mat srcWithAlpha;
    if (src.channels() < 4)
        cvtColor(src, srcWithAlpha, CV_BGR2BGRA);
    else
        srcWithAlpha = src;

     cv::Mat temp; // make the same cv::Mat
     cvtColor(srcWithAlpha, temp, CV_BGRA2RGBA);
     temp = srcWithAlpha;

     QImage dest((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_ARGB32);


     QImage dest2(dest);
     dest2.detach(); // enforce deep copy

     return dest2;
}
