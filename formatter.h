#ifndef FORMATTER_H
#define FORMATTER_H

#include "data.h"

class Formatter {
public:
    virtual QString format(const Stream &s) const = 0;
};


class DoubleLineFormatter : public Formatter {
public:
    DoubleLineFormatter();
    QString format(const Stream &s) const override;

    QString jpLineFormat;
    QString cnLineFormat;
};

#endif // FORMATTER_H
