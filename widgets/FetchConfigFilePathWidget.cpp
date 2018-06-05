#include "FetchConfigFilePathWidget.h"
#include "ui_FetchConfigFilePathWidget.h"

FetchConfigFilePathWidget::FetchConfigFilePathWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FetchConfigFilePathWidget)
{
    ui->setupUi(this);
}

FetchConfigFilePathWidget::~FetchConfigFilePathWidget()
{
    delete ui;
}
