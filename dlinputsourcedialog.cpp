#include "dlinputsourcedialog.h"
#include "ui_dlinputsourcedialog.h"
#include <QFileDialog>
#include <QTextStream>
#include <QStringConverter>
#include <QRegularExpression>
#include "utils.h"
#include "encoding.h"

bool containsJapaneseCharacters(const QString &str);
QString generateRegex(const QStringList &lines);
QString regexSubtract(const QString& reg,
                      const std::vector<std::pair<QString, QStringList>>& notWanted);
bool matchAnyPrefix(const QString &pattern, const QStringList &lines);
bool matchAllPrefix(const QString &pattern, const QStringList &lines);

DoubleLineInputSourceDialog::DoubleLineInputSourceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DoubleLineInputSourceDialog)
    , previewIsValid(false)

{
    ui->setupUi(this);
    QStringList encodingList;
    for (auto e: encoding::supportedEncodings) {
        encodingList.append(e);
    }
    ui->previewEncodingComboBox->addItems(encodingList);
    connect(ui->previewEncodingComboBox, &QComboBox::currentIndexChanged, this, &DoubleLineInputSourceDialog::selectInputEncoding);
    connect(ui->previewFileComboBox, &QComboBox::currentIndexChanged, this, &DoubleLineInputSourceDialog::selectInputFile);


    selectedEncoding = encoding::supportedEncodings[0];
    connect(ui->inputSelectButton, &QPushButton::clicked, this, &DoubleLineInputSourceDialog::selectInputSource);

    connect(ui->detectFormatButton, &QPushButton::clicked, this, &DoubleLineInputSourceDialog::detectFormat);
    connect(ui->testStreamButton, &QPushButton::clicked, this, &DoubleLineInputSourceDialog::testStreamOutput);

}

DoubleLineInputSourceDialog::~DoubleLineInputSourceDialog()
{
    delete ui;
}

void DoubleLineInputSourceDialog::selectInputSource()
{
    QString dir = QFileDialog::getExistingDirectory(
        this,
        "选择目录",
        selectedDir.isEmpty() ? QDir::homePath() : selectedDir,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    selectedDir = dir;
    ui->inputDirEdit->setText(selectedDir);


    const auto files = utils::loadFilesFromDirectory(dir, true);
    ui->previewFileComboBox->clear();
    QStringList filesList;
    for(auto &f: files) {
        filesList.append(f);
    }
    ui->previewFileComboBox->addItems(filesList);


    if (files.size() > 0) {
        selectedFile = files[0];
        ui->previewFileComboBox->setCurrentIndex(0);
        updatePreview();
    }
}

void DoubleLineInputSourceDialog::selectInputEncoding(int index)
{
    selectedEncoding = ui->previewEncodingComboBox->itemText(index);
    updatePreview();
}

void DoubleLineInputSourceDialog::selectInputFile(int index)
{
    selectedFile = ui->previewFileComboBox->itemText(index);
    updatePreview();
}

void DoubleLineInputSourceDialog::updatePreview()
{
    previewIsValid = false;
    if (selectedFile.isEmpty()) {
        return;
    }
    QFile file(selectedFile);
    if (!file.open(QIODevice::ReadOnly)) {
        ui->previewEdit->setText("文件打开失败");
        return;
    }
    QByteArray fileContent = file.readAll();
    auto res = encoding::convert(fileContent, encoding::stringToEnum(selectedEncoding.toStdString()), encoding::Encoding::UTF8);
    if (!res.has_value()) {
        ui->previewEdit->setText("编码转换失败");
        return;
    }
    QByteArray utf8Content = res.value();
    QString text = QString::fromUtf8(utf8Content);
    ui->previewEdit->setText(text);
    previewIsValid = true;
    detectFormat();
}



void DoubleLineInputSourceDialog::testStreamOutput()
{
    if (!previewIsValid) {
        return;
    }

    QString text = ui->previewEdit->toPlainText();
    QString originalRegexStr = ui->originalRegexEdit->text();
    QString translationRegexStr = ui->translationRegexEdit->text();
    QString commentRegexStr = ui->commentRegexEdit->text();

    QRegularExpression jreg(QString("(?<tag>%1)(?<content>.*)").arg(originalRegexStr));
    QRegularExpression creg(QString("(?<tag>%1)(?<content>.*)").arg(translationRegexStr));
    QRegularExpression oreg("(?!)");
    if (!commentRegexStr.isEmpty()) {
        oreg = QRegularExpression(QString("(?<tag>%1)(?<content>.*)").arg(commentRegexStr));
    }



    QStringList jtags, jtexts, ctags, ctexts;
    QTextStream stream(&text);
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        if (auto m = oreg.match(line); m.hasMatch()) {
            continue;
        }
        if (auto m = jreg.match(line); m.hasMatch()) {
            QString tag = m.captured("tag");
            QString content = m.captured("content");
            jtags.append(tag);
            jtexts.append(content);
        } else if (auto m = creg.match(line); m.hasMatch()) {
            QString tag = m.captured("tag");
            QString content = m.captured("content");
            ctags.append(tag);
            ctexts.append(content);
        }
    }

    if (jtags.size() != ctags.size()) {
        ui->streamOutputEdit->setText(QString("原文与译文行数不一致，原文： %1，译文： %2").arg(jtags.size(), ctags.size()));
        return;
    }

    QString streamPreview;
    for(int i = 0; i < jtags.size() && i < 10; i++) {
        streamPreview.append(QString("jtag : %1\n").arg(jtags[i]));
        streamPreview.append(QString("jtext: %1\n").arg(jtexts[i]));
        streamPreview.append(QString("ctag : %1\n").arg(ctags[i]));
        streamPreview.append(QString("ctext: %1\n").arg(ctexts[i]));
        streamPreview.append("\n");
    }
    ui->streamOutputEdit->setText(streamPreview);
}

void DoubleLineInputSourceDialog::detectFormat()
{
    if (!previewIsValid) {
        return;
    }

    QString text = ui->previewEdit->toPlainText();
    QTextStream stream(&text);
    QStringList lines;
    while(!stream.atEnd()) {
        lines.append(stream.readLine());
    }
    QString commentRegexStr = ui->commentRegexEdit->text();
    QRegularExpression oreg("(?!)");
    if (!commentRegexStr.isEmpty()) {
        oreg = QRegularExpression(QString("(?<tag>%1)(?<content>.*)").arg(commentRegexStr));
    }

    int startLine = 0;
    const int maxCount = 30;
    QStringList rlines, tlines;
    for (auto lineNumber = startLine; lineNumber < lines.size() - 1 && rlines.size() < maxCount; lineNumber++) {
        const auto thisLine = lines[lineNumber].trimmed();

        if (oreg.match(thisLine).hasMatch()) {
            continue;
        }

        if (containsJapaneseCharacters(thisLine)) {
            if (rlines.size() == tlines.size()) {
                rlines.append(thisLine);
            } else {
                tlines.append(thisLine);
            }
        }
    }
    if (rlines.size() > tlines.size()) {
        rlines.pop_back();
    }
    if (rlines.size() < 2 || tlines.size() < 2 || rlines.size() != tlines.size()) {
        ui->streamOutputEdit->setText("格式自动识别失败");
        return;
    }

    auto rRegStr = generateRegex(rlines);
    auto tRegStr = generateRegex(tlines);

    rRegStr = regexSubtract(rRegStr, {
                                         {tRegStr, tlines}
                                     });

    tRegStr = regexSubtract(tRegStr, {
                                         {rRegStr, rlines}
                                     });

    if (matchAnyPrefix(rRegStr, tlines) || matchAnyPrefix(tRegStr, rlines)) {
        ui->streamOutputEdit->setText("格式自动识别失败");
        return;
    }

    ui->originalRegexEdit->setText(rRegStr);
    ui->translationRegexEdit->setText(tRegStr);
    testStreamOutput();
}

bool containsJapaneseCharacters(const QString &str) {
    static QRegularExpression japaneseRegex("[\\p{Script=Hiragana}\\p{Script=Katakana}\\p{Script=Han}ー]");
    // Regular expression to match Hiragana, Katakana, Kanji (Han), and the long vowel mark (ー)
    return japaneseRegex.match(str).hasMatch();
}

std::unordered_map<QString, QString> createReverseMap(const std::unordered_map<QString, QString> &map) {
    std::unordered_map<QString, QString> reverseMap;
    for (const auto &[key, value] : map) {
        reverseMap[value] = key;  // Swap key and value
    }
    return reverseMap;
}


bool hasAlphaNumPrefix(const QStringList &lines) {
    static QRegularExpression alphaNumReg("^[A-Za-z0-9]");
    for (const auto &line : lines) {
        if (alphaNumReg.match(line).hasMatch()) {
            return true;
        }
    }
    return false;
}

QString commonPrefix(const QStringList &lines) {
    if (lines.isEmpty()) return "";
    QString prefix = lines[0];
    for (const auto &line : lines) {
        int i = 0;
        while (i < prefix.size() && i < line.size() && !(line[i] >= '0' && line[i] <= '9') && prefix[i] == line[i]) {
            i++;
        }
        prefix = prefix.left(i);
        if (prefix.isEmpty()) break;
    }
    return prefix;
}

bool matchAllPrefix(const QString &pattern, const QStringList &lines) {
    QRegularExpression reg(QStringLiteral("^(%1)(.*)").arg(pattern));
    for (const auto &line : lines) {
        if (!reg.match(line).hasMatch()) {
            return false;
        }
    }
    return true;
}
bool matchAnyPrefix(const QString &pattern, const QStringList &lines) {
    QRegularExpression reg(QStringLiteral("^(%1)(.*)").arg(pattern));
    for (const auto &line : lines) {
        if (reg.match(line).hasMatch()) {
            return true;
        }
    }
    return false;
}

QString escapeRegExp(const QString &str) {
    return QRegularExpression::escape(str);
}

QString generateRegex(const QStringList &lines) {
    static const std::unordered_map<QString, QString> paraMap = {
        {"{", "}"}, {"[", "]"}, {"(", ")"}, {"<", ">"},
        {"《", "》"}, {"【", "】"}, {"（", "）"}
    };
    static const auto paraReverseMap = createReverseMap(paraMap);

    QString regStr;
    std::vector<QString> openingPar;

    while (true) {
        QRegularExpression reg(QStringLiteral("^(%1)(.*)").arg(regStr));
        QStringList reminders;

        for (const auto &line : lines) {
            auto m = reg.match(line);
            if (!m.hasMatch()) {
                return "";
            }
            reminders.push_back(m.captured(2));
        }

        if (hasAlphaNumPrefix(reminders)) {
            regStr += "[A-Za-z0-9]+";
            continue;
        }

        QString candidate = commonPrefix(reminders);
        if (!candidate.isEmpty()) {
            if (paraMap.find(candidate) != paraMap.end()) {
                openingPar.push_back(candidate);
            } else if (paraReverseMap.find(candidate) != paraReverseMap.end()) {
                QString left = paraReverseMap.at(candidate);
                auto it = std::find(openingPar.rbegin(), openingPar.rend(), left);
                if (it != openingPar.rend()) {
                    openingPar.erase((it + 1).base());
                }
            }
            regStr += escapeRegExp(candidate);
            continue;
        }

        if (!openingPar.empty()) {
            QString left = openingPar.back();
            openingPar.pop_back();
            QString right = paraMap.at(left);
            if (matchAllPrefix(regStr + ".*?" + right, lines)) {
                regStr += ".*?" + escapeRegExp(right);
                continue;
            }
        }
        break;
    }
    return regStr;
}

QString regexSubtract(const QString& reg,
                      const std::vector<std::pair<QString, QStringList>>& notWanted) {
    QStringList subtractSet;
    QString prefix = reg.isEmpty() ? "^" : "";
    QString suffix = reg.isEmpty() ? "(?=.)" : "";

    for (const auto& [otherReg, otherLines] : notWanted) {
        if (!otherReg.isEmpty() && matchAnyPrefix(reg, otherLines)) {
            subtractSet.append(otherReg);
        }
    }

    if (!subtractSet.isEmpty()) {
        QString toRemove = QString("(%1)").arg(subtractSet.join(")|("));
        return QString("%1(?!%2)%3%4").arg(prefix, toRemove, reg, suffix);
    }

    return QString("%1%2%3").arg(prefix, reg, suffix);
}
