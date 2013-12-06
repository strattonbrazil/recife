#ifndef UTILS_H
#define UTILS_H

#include <QImage>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

QImage convertMatToQImage(cv::Mat const& src);

#endif // UTILS_H
