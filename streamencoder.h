#ifndef STREAMENCODER_H
#define STREAMENCODER_H

#include <QStringList>
#include "data.h"

class StreamEncoder
{
public:
    virtual QString encode(const Stream &stream) = 0;
};

class DoubleLineStreamEncoder: public StreamEncoder
{
public:
    DoubleLineStreamEncoder();
    QString encode(const Stream &stream) override;
};

#endif // STREAMENCODER_H
