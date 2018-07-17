#include "GroupDetailWidget.h"
#include "ui_GroupDetailWidget.h"

GroupDetailWidget::GroupDetailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupDetailWidget)
{
    ui->setupUi(this);
}

GroupDetailWidget::~GroupDetailWidget()
{
    delete ui;
}
