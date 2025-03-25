#include "outputsink.h"
#include <QTextStream>
#include <QFile>

DirectoryOutputSink::DirectoryOutputSink(QString dirpath)
    : dir(dirpath)
{
    if(!dir.exists()) {
        dir.mkpath(".");
    }
}

void DirectoryOutputSink::output(const Context &ctx, const QString &text) const
{
    QString filepath = dir.filePath(ctx.filename);
    QFile file(filepath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    QTextStream out(&file);
    out << text;
    file.close();
}
