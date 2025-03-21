#include "app.h"
#include "./ui_app.h"
#include "dlinputsourcedialog.h"

app::app(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::app)
{
    ui->setupUi(this);
    connect(ui->inputSourceButton, &QPushButton::clicked, this, &app::openInputSourceDialog);
}



app::~app()
{
    delete ui;
}

void app::openInputSourceDialog()
{
    DoubleLineInputSourceDialog d;
    d.exec();
}
