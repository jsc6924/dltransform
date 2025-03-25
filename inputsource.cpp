#include "inputsource.h"
#include <QRegularExpression>
#include <QMessageBox>
#include <QFile>
#include "utils.h"
#include "encoding.h"

DoubleLineInputSource::DoubleLineInputSource(const QString &dir, const QString &encoding, const QString &jreg, const QString &creg, const QString &ignoreReg)
    :InputSource(dir, encoding),
    jreg(QString("(?<tag>%1)(?<content>.*)").arg(jreg)),
    creg(QString("(?<tag>%1)(?<content>.*)").arg(creg)),
    ignoreReg(ignoreReg.isEmpty() ? "(?!)" : QString("(?<tag>%1)(?<content>.*)").arg(ignoreReg))
{

}

Stream DoubleLineInputSource::stream(const Context &ctx) const
{
    QFile file(dir.filePath(ctx.filename));
    if(!file.exists()) {
        QMessageBox::information(nullptr, "错误", QString("DoubleLineInputSource::stream 文件不存在%1").arg(dir.filePath(ctx.filename)));
        return {};
    }
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(nullptr, "错误", QString("DoubleLineInputSource::stream 文件无法打开%1").arg(dir.filePath(ctx.filename)));
        return {};
    }
    QByteArray fileContent = file.readAll();
    auto res = encoding::convert(fileContent, encoding::stringToEnum(this->encoding.toStdString()), encoding::Encoding::UTF8);
    if (!res.has_value()) {
        QMessageBox::information(nullptr, "错误", QString("DoubleLineInputSource::stream 编码转换失败%1").arg(dir.filePath(ctx.filename)));
        return {};
    }
    QByteArray utf8Content = res.value();
    QString text = QString::fromUtf8(utf8Content);

    Stream s;
    s.addChannel("jtags");
    s.addChannel("jtexts");
    s.addChannel("ctags");
    s.addChannel("ctexts");
    QTextStream stream(&text);
    QStringList frame;
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        if (auto m = ignoreReg.match(line); m.hasMatch()) {
            continue;
        }
        if (auto m = jreg.match(line); m.hasMatch()) {
            if (!frame.isEmpty()) {
                QMessageBox::information(nullptr, "错误", "DoubleLineInputSource::stream 标签格式错误1");
            }
            QString tag = m.captured("tag");
            QString content = m.captured("content");
            frame.append(tag);
            frame.append(content);
        } else if (auto m = creg.match(line); m.hasMatch()) {
            QString tag = m.captured("tag");
            QString content = m.captured("content");
            frame.append(tag);
            frame.append(content);
            if(!s.addFrame(frame)) {
                QMessageBox::information(nullptr, "错误", "DoubleLineInputSource::stream 标签格式错误2");
            }
            frame.clear();
        }
    }

    return s;
}

void InputSource::forEach(std::function<void (const QString &)> cb)
{
    const auto files = utils::loadFilesFromDirectory(dir, true);
    for(auto &f: files) {
        cb(f);
    }
}
