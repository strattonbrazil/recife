#ifndef UTILS_H
#define UTILS_H

#include <QImage>
#include <QLayout>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QMetaProperty>
#include <QSet>

QImage convertMatToQImage(cv::Mat const& src);
cv::Mat addAlphaChannel(cv::Mat const& src);
void clearLayout(QLayout* layout, QSet<QWidget*> preserveWidgets=QSet<QWidget*>());
//QMetaProperty findProperty(QObject* obj, const char* name);

float interpolate(float x, float x1, float y1, float x2, float y2);

QPointF evalPointF(QString s);

QPoint stringToPoint(QString s);
QPointF stringToPointF(QString s);

#endif // UTILS_H
