#ifndef INPUTSOURCEDIALOG_H
#define INPUTSOURCEDIALOG_H

#include <QDialog>

namespace Ui {
class InputSourceDialog;
}

class InputSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputSourceDialog(QWidget *parent = nullptr);
    ~InputSourceDialog();

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
    Ui::InputSourceDialog *ui;
    QString selectedDir;
    QString selectedFile;
    QString selectedEncoding;
    bool previewIsValid;
};

#endif // INPUTSOURCEDIALOG_H
