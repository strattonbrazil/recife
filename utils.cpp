#include "utils.h"

#include <opencv2/ocl/ocl.hpp>

QImage convertMatToQImage(cv::Mat const& src)
{
     cv::Mat temp; // make the same cv::Mat
     cvtColor(src, temp, CV_BGR2RGB); // cvtColor Makes a copt, that what i need
     QImage dest((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     QImage dest2(dest);
     dest2.detach(); // enforce deep copy
     return dest2;
}

cv::Mat addAlphaChannel(cv::Mat const& src)
{
    cv::Mat dst = cv::Mat(src.rows, src.cols, CV_8UC4);
    int from_to[] = { 0,0, 1,1, 2,2, 3,3 };
    cv::mixChannels(&src, 3, &dst,1, from_to, 4);

    return dst;
}
