#include "utils.h"

#include <opencv2/ocl/ocl.hpp>

#include <iostream>

QImage convertMatToQImage(cv::Mat const& unformattedSrc)
{
    // make sure there's an alpha channel
    cv::Mat4b src;
    if (unformattedSrc.channels() < 4)
        cvtColor(unformattedSrc, src, CV_RGB2RGBA);
    else
        src = unformattedSrc;

    QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
    for (int y = 0; y < src.rows; ++y) {
        const cv::Vec4b *srcrow = src[y];
        QRgb *destrow = (QRgb*)dest.scanLine(y);
        for (int x = 0; x < src.cols; ++x) {
            destrow[x] = qRgba(srcrow[x][2], srcrow[x][1], srcrow[x][0], srcrow[x][3]);
        }
    }
    return dest;

            /*

     cv::Mat temp; // make the same cv::Mat
     cvtColor(srcWithAlpha, temp, CV_BGRA2RGBA);
     temp = srcWithAlpha;

     QImage dest((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_ARGB32);


     QImage dest2(dest);
     dest2.detach(); // enforce deep copy
     */

//     return dest2;
     //QImage foo;
     //foo.load("/home/josh.stratton/Pictures/alpha.png");
     //return foo;
}
