#ifndef INPUTSOURCE_H
#define INPUTSOURCE_H

#include <QString>
#include <QRegularExpression>
#include <QDir>
#include <functional>
#include "data.h"


class InputSource
{
public:
    InputSource(const QString &dir, const QString &encoding)
        : dir(dir), encoding(encoding)
    {}
    void forEach(std::function<void(const QString&)> cb);
    virtual Stream stream(const Context &ctx) const = 0;
protected:
    QDir dir;
    QString encoding;
};

class SimpleInputSource : public InputSource
{
public:
    virtual QString text(const Context &ctx) const = 0;
};

class SubInputSource : public InputSource {
public:
    virtual Stream stream(const Context &ctx) const = 0;
};

class DoubleLineInputSource : public InputSource {
public:
    DoubleLineInputSource(const QString &dir, const QString &encoding,
                          const QString &jreg, const QString &creg, const QString &ignoreReg);
    Stream stream(const Context &ctx) const override;
protected:
    QRegularExpression jreg, creg, ignoreReg;
};

#endif // INPUTSOURCE_H
