#include "CalendarWeekWidget.h"
#include "ui_CalendarWeekWidget.h"

CalendarWeekWidget::CalendarWeekWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarWeekWidget)
{
    ui->setupUi(this);
}

CalendarWeekWidget::~CalendarWeekWidget()
{
    delete ui;
}
