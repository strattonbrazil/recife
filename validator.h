#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <QValidator>

class PointFValidator : public QValidator
{
    QValidator::State validate(QString &input, int &position) const;
    void fixup(QString &input) const;
};

#endif // VALIDATOR_H
