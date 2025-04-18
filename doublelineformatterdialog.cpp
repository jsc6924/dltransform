#include "doublelineformatterdialog.h"
#include "ui_doublelineformatterdialog.h"

DoubleLineFormatterDialog::DoubleLineFormatterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DoubleLineFormatterDialog)
{
    ui->setupUi(this);
    ui->jpFormatEdit->setText(formatter.jpLineFormat);
    ui->cnFormatEdit->setText(formatter.cnLineFormat);

    connect(ui->jpFormatEdit, &QLineEdit::textEdited, this, &DoubleLineFormatterDialog::updateJpFormat);
    connect(ui->cnFormatEdit, &QLineEdit::textEdited, this, &DoubleLineFormatterDialog::updateCnFormat);
    connect(ui->cancelButton, &QPushButton::clicked, this, &DoubleLineFormatterDialog::reject);
    connect(ui->saveButton, &QPushButton::clicked, this, &DoubleLineFormatterDialog::accept);
}

void DoubleLineFormatterDialog::updateJpFormat(const QString &s) {
    formatter.jpLineFormat = s;
}

void DoubleLineFormatterDialog::updateCnFormat(const QString &s) {
    formatter.cnLineFormat = s;
}

std::shared_ptr<DoubleLineFormatter> DoubleLineFormatterDialog::getFormatter() const
{
    return std::make_shared<DoubleLineFormatter>(formatter);
}

DoubleLineFormatterDialog::~DoubleLineFormatterDialog()
{
    delete ui;
}
