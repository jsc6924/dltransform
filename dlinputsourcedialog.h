#ifndef DLINPUTSOURCEDIALOG_H
#define DLINPUTSOURCEDIALOG_H

#include <QDialog>

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


private:
    Ui::DoubleLineInputSourceDialog *ui;
    QString selectedDir;
    QString selectedFile;
    QString selectedEncoding;
    bool previewIsValid;
};

#endif // DLINPUTSOURCEDIALOG_H
