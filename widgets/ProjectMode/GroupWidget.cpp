#include "GroupWidget.h"
#include "ui_groupwidget.h"

GroupWidget::GroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupWidget)
{
    ui->setupUi(this);

    this->groupDetailWidget = new GroupDetailWidget(this);
    ui->stackedWidget->addWidget(this->groupDetailWidget);
    ui->stackedWidget->setCurrentWidget(this->groupDetailWidget);

    this->mainSplitter = new QSplitter(this);
    this->mainSplitter->addWidget(ui->listView);
    this->mainSplitter->addWidget(ui->stackedWidget);
    ui->horizontalLayout->addWidget(this->mainSplitter);
}

GroupWidget::~GroupWidget()
{
    delete ui;
}
