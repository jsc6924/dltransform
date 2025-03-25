#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include "inputsource.h"
#include "formatter.h"
#include "data.h"
#include "outputsink.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class app;
}
QT_END_NAMESPACE

class app : public QMainWindow
{
    Q_OBJECT

public:
    app(QWidget *parent = nullptr);
    ~app();

    void openInputSourceDialog();
    void run();

private:
    Ui::app *ui;

    std::shared_ptr<InputSource> selectedInput;
    std::shared_ptr<Formatter> selecedFormatter;
    std::shared_ptr<OutputSink> selectedOutput;
};
#endif // APP_H
