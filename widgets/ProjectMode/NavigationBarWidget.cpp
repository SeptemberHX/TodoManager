#include "NavigationBarWidget.h"
#include "ui_NavigationBarWidget.h"

NavigationBarWidget::NavigationBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigationBarWidget)
{
    ui->setupUi(this);
}

NavigationBarWidget::~NavigationBarWidget()
{
    delete ui;
}
