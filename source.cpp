#include "source.h"

#include <QStringList>
#include <QVector>
#include <stdexcept>
#include <QFile>
#include <QDir>
#include <opencv2/highgui/highgui.hpp>

ImageSource::ImageSource(QString fileName)
{
    // TODO: use a namebase utility function here
    _label = fileName.split("/").last();
    _imagePath = fileName;
}

QString ImageSource::label()
{
    return _label;
}

Mat ImageSource::render(int frame)
{
    return cv::imread(_imagePath.toStdString(), CV_LOAD_IMAGE_COLOR);
}

class ImageHandler : public FileHandler
{
    QSharedPointer<Source> process(QString fileName)
    {
        if (fileName.endsWith(".jpg")) {
            return QSharedPointer<Source>(new ImageSource(fileName));
        }
        else if (fileName.endsWith(".png")) {
            return QSharedPointer<Source>(new ImageSource(fileName));
        }

        return QSharedPointer<Source>(0);
    }
};

QVector<FileHandler*> fileHandlers;

void validate()
{
    if (fileHandlers.size() == 0) {
        ImageHandler* imageHandler = new ImageHandler();

        fileHandlers.append(imageHandler);
    }
}

QSharedPointer<Source> Source::getSource(QString fileName)
{
    validate();

    foreach(FileHandler* handler, fileHandlers) {
        QSharedPointer<Source> src = handler->process(fileName);
        if (!src.isNull()) {
            src->_effectsList = 0;
            return src;
        }
    }

    throw std::runtime_error("failed to find appropriate importer");
}

EffectsModel* Source::effectsModel()
{
    if (_effectsList == 0)
        _effectsList = new EffectsModel();
    return _effectsList;
}

void Source::addEffect(QSharedPointer<Effect> effect)
{
    _effectsList->addEffect(effect);
}
