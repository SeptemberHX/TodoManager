#include "GroupWidget.h"
#include "ui_groupwidget.h"

GroupWidget::GroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupWidget)
{
    ui->setupUi(this);
}

GroupWidget::~GroupWidget()
{
    delete ui;
}
