#ifndef FORMATTER_H
#define FORMATTER_H

#include "data.h"

class Formatter {
public:
    virtual QString format(const Stream &s) const = 0;
};


class DefaultFormatter : public Formatter {
public:
    QString format(const Stream &s) const override;
};

#endif // FORMATTER_H
