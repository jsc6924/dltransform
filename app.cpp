#include "app.h"
#include "./ui_app.h"
#include "dlinputsourcedialog.h"
#include <QFileInfo>

app::app(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::app)
{
    ui->setupUi(this);
    connect(ui->inputSourceButton, &QPushButton::clicked, this, &app::openInputSourceDialog);
    connect(ui->runButton, &QPushButton::clicked, this, &app::run);

    selecedFormatter = std::make_shared<DefaultFormatter>();
    selectedOutput = std::make_shared<DirectoryOutputSink>("test_output/");
}



app::~app()
{
    delete ui;
}

void app::openInputSourceDialog()
{
    DoubleLineInputSourceDialog d;
    if(d.exec()) {
        auto input =  d.getInputSource();
        if (input.has_value()) {
            selectedInput = input.value();
            ui->runButton->setEnabled(true);
        }
    }
}

void app::run()
{
    if (!selectedInput.get()) {
        return;
    }
    selectedInput.get()->forEach([&](const QString &filepath) {
        QFileInfo fileInfo(filepath);
        QString fileName = fileInfo.fileName();
        Context ctx;
        ctx.filename = fileName;

        Stream stream = selectedInput.get()->stream(ctx);
        QString outText = selecedFormatter.get()->format(stream);
        selectedOutput.get()->output(ctx, outText);
    });
}


