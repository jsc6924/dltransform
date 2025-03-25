#ifndef OUTPUTSINK_H
#define OUTPUTSINK_H

#include <QString>
#include <QDir>
#include "data.h"

class OutputSink
{
public:
    virtual void output(const Context &ctx, const QString &text) const = 0;
};

class DirectoryOutputSink : public OutputSink {
public:
    DirectoryOutputSink(QString dir);
    virtual void output(const Context &ctx, const QString &text) const override;
protected:
    QDir dir;
};


#endif // OUTPUTSINK_H
