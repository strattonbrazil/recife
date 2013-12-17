#ifndef UTILS_H
#define UTILS_H

#include <QImage>
#include <QLayout>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

QImage convertMatToQImage(cv::Mat const& src);
cv::Mat addAlphaChannel(cv::Mat const& src);
void clearLayout(QLayout* layout);

#endif // UTILS_H
