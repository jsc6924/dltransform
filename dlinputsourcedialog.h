#ifndef DLINPUTSOURCEDIALOG_H
#define DLINPUTSOURCEDIALOG_H

#include <QDialog>
#include "inputsource.h"
#include <optional>

namespace Ui {
class DoubleLineInputSourceDialog;
}

class DoubleLineInputSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DoubleLineInputSourceDialog(QWidget *parent = nullptr);
    ~DoubleLineInputSourceDialog();

    void selectInputSource();
    QString getSelectedDir()
    {
        return selectedDir;
    }

    void selectInputEncoding(int index);
    void selectInputFile(int index);
    void updatePreview();
    void testStreamOutput();
    void detectFormat();
    std::optional<std::shared_ptr<InputSource>> getInputSource() const;


private:
    Ui::DoubleLineInputSourceDialog *ui;
    QString selectedDir;
    QString selectedFile;
    QString selectedEncoding;
    QString selectedJreg, selectedCreg, selectedIgnoreReg;
    bool previewIsValid;
    bool streamOutputIsValid;
};

#endif // DLINPUTSOURCEDIALOG_H
