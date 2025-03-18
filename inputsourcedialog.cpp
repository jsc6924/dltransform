#include "inputsourcedialog.h"
#include "ui_inputsourcedialog.h"
#include <QFileDialog>

InputSourceDialog::InputSourceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InputSourceDialog)
{
    ui->setupUi(this);
    connect(ui->inputSelectButton, &QPushButton::clicked, this, &InputSourceDialog::selectInputSource);
}

InputSourceDialog::~InputSourceDialog()
{
    delete ui;
}

void InputSourceDialog::selectInputSource()
{
    QString dir = QFileDialog::getExistingDirectory(
        this,
        "选择目录",
        QDir::homePath(),  // Default directory
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    selectedDir = dir;
    qDebug() << QString("Selected: %1").arg(selectedDir);


}
