#ifndef SOURCE_H
#define SOURCE_H

#include <QSharedPointer>
#include <QVector>

class FileHandler;

class Source
{
public:
    virtual QString label() = 0;

    static QSharedPointer<Source> getSource(QString fileName);
};

class ImageSource : public Source
{
public:
    ImageSource(QString fileName);
    QString label();
private:
    QString _label;
};



class FileHandler
{
public:
    virtual QSharedPointer<Source> process(QString fileName) = 0;
};

#endif // SOURCE_H
