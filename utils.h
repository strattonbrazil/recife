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
QMetaProperty findProperty(QObject* obj, const char* name);

// TODO: these are temporary functions that will be replaced later by a real interpreter
QPoint stringToPoint(QString s);
QPointF stringToPointF(QString s);

#endif // UTILS_H
