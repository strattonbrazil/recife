#include "effectsmodel.h"

#include <opencv2/ocl/ocl.hpp>
#include "utils.h"

EffectsModel::EffectsModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int EffectsModel::rowCount(const QModelIndex &parent) const
{
    return _effects.size();
}

QVariant EffectsModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    return _effects[index.row()]->label();
}

void EffectsModel::addEffect(QSharedPointer<Effect> effect)
{
    _effects.append(effect);

    QModelIndex topLeft = createIndex(0, 0);
    emit(dataChanged(topLeft, topLeft));
}
#include <iostream>
Mat EffectsModel::process(Mat in, int frame)
{
    for (int i = _effects.size() - 1; i >= 0; i--) {
        if (_effects[i]->requires() == Effect::REQUIRE_RGB && in.channels() < 3) {
            std::cout << "gray to color" << std::endl;
            Mat convertedImg;
            cvtColor(in, convertedImg, CV_GRAY2RGB);
            in = convertedImg;
        }
        else if (_effects[i]->requires() == Effect::REQUIRE_RGBA && in.channels() < 3) {
            std::cout << "gray to color/alpha" << std::endl;
            Mat convertedImg;
            cvtColor(in, convertedImg, CV_GRAY2RGB);
            in = convertedImg;
        }
        else if (_effects[i]->requires() == Effect::REQUIRE_RGBA && in.channels() < 4) {
            Mat convertedImg;
            cvtColor(in, convertedImg, CV_RGB2RGBA);
            in = convertedImg;
            std::cout << "color to color/alpha" << std::endl;
        }

        in = _effects[i]->process(in, frame);

        uchar foo = in.at<Vec4b>(10,10).val[3];
        if (foo < 128)
            std::cout << "transparent" << std::endl;
        else
            std::cout << "opaque" << std::endl;
    }

    return in;
}
