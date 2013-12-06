#include "source.h"

#include <QStringList>
#include <QVector>
#include <stdexcept>
#include <QFile>
#include <QDir>

ImageSource::ImageSource(QString fileName)
{
    // TODO: use a namebase utility function here
    _label = fileName.split("/").last();
}

QString ImageSource::label()
{
    return _label;
}

class ImageHandler : public FileHandler
{
    QSharedPointer<Source> process(QString fileName)
    {
        if (fileName.endsWith(".jpg")) {
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
        if (!src.isNull())
            return src;
    }

    throw std::runtime_error("failed to find appropriate importer");
}
