#include "TagModeWidget.h"
#include "ui_TagModeWidget.h"

TagModeWidget::TagModeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagModeWidget)
{
    ui->setupUi(this);

    this->todoListWidget = new TodoListWidget(this, TodoListWidgetMode::INBOX);
    this->mainSplitter = new QSplitter(this);
    this->mainSplitter->addWidget(ui->listView);
    this->mainSplitter->addWidget(this->todoListWidget);
    this->mainSplitter->setStretchFactor(0, 1);
    this->mainSplitter->setStretchFactor(1, 3);
    ui->horizontalLayout->addWidget(this->mainSplitter);
}

TagModeWidget::~TagModeWidget()
{
    delete ui;
}
