#include "utils.h"

#include <opencv2/ocl/ocl.hpp>
#include <QStringList>
#include <QScriptEngine>
#include <exception>
#include <stdexcept>

#include <iostream>
//#include <libguile.h>

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

void clearLayout(QLayout *layout, QSet<QWidget*> preserveWidgets)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            QWidget* widget = item->widget();
            if (!preserveWidgets.contains(widget))
                delete widget;
        }
        delete item;
    }
}

/*
QMetaProperty findProperty(QObject* obj, const char* name)
{
    const QMetaObject *metaobject = obj->metaObject();
    int propertyCount = metaobject->propertyCount();
    for (int i = 0; i < propertyCount; i++) {
        QMetaProperty metaproperty = metaobject->property(i);
        //const char *name = metaproperty.name();
        if (strcmp(name, metaproperty.name()) == 0) // not a layer property
            return metaproperty;
    }
}
*/

float interpolate(float x, float x1, float y1, float x2, float y2)
{
    return y1 + (y2-y1)*((x-x1)/(x2-x1));
}

/*
bool guileInitialized = false;
void initGuile()
{
    if (!guileInitialized) {
        scm_init_guile();
        guileInitialized = true;
    }
}
*/


QPointF evalPointF(QString s)
{
    //initGuile();

    QScriptEngine engine;
    QString program = "function frame() { return 1; }\n\n" + s;

    if (!engine.canEvaluate(program))
        throw std::runtime_error("can't evaluate point");

    QScriptValue value = engine.evaluate(program);

    QStringList pair = value.toString().split(",");
    if (pair.length() < 2)
        throw std::runtime_error("invalid pointf string");

    return QPointF(pair[0].toFloat(), pair[1].toFloat());
}



QPoint stringToPoint(QString s)
{
    QStringList tokens = s.replace("(","").replace(")","").replace(" ", "").split(",");
    if (tokens.length() < 2)
        return QPoint(0,0);
    return QPoint(tokens[0].toInt(), tokens[1].toInt());
}

QPointF stringToPointF(QString s)
{
    QStringList tokens = s.replace("(","").replace(")","").replace(" ", "").split(",");
    if (tokens.length() < 2)
        return QPointF(0,0);
    return QPointF(tokens[0].toFloat(), tokens[1].toFloat());
}

void setButtonKeyFrame(QPushButton *button, bool add)
{
    if (add) { // set the button to create key frames
        button->setIcon(QIcon(":icons/keyframe.png"));
        button->setToolTip("Set keyframe");
        button->setProperty("add_keyframe", true);
    } else { // set the button to remove key frames
        button->setIcon(QIcon(":icons/rm_keyframe.png"));
        button->setToolTip("Remove keyframe");
        button->setProperty("add_keyframe", false);
    }
}
