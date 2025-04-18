#ifndef DOUBLELINEFORMATTERDIALOG_H
#define DOUBLELINEFORMATTERDIALOG_H

#include <QDialog>
#include "formatter.h"

namespace Ui {
class DoubleLineFormatterDialog;
}

class DoubleLineFormatterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DoubleLineFormatterDialog(QWidget *parent = nullptr);
    ~DoubleLineFormatterDialog();

    void updateJpFormat(const QString &s);
    void updateCnFormat(const QString &s);

    std::shared_ptr<DoubleLineFormatter> getFormatter() const;

    DoubleLineFormatter formatter;


private:
    Ui::DoubleLineFormatterDialog *ui;
};

#endif // DOUBLELINEFORMATTERDIALOG_H
