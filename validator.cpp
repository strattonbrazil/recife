#include "validator.h"
#include <iostream>
#include "utils.h"

QValidator::State PointFValidator::validate(QString &input, int &position) const
{
    try {
        evalPointF(input);
    } catch (std::exception &e) {
        return QValidator::Intermediate;
    }

    return QValidator::Acceptable;
}

void PointFValidator::fixup(QString &input) const
{
}
