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

private:
    Ui::InputSourceDialog *ui;
    QString selectedDir;
};

#endif // INPUTSOURCEDIALOG_H
