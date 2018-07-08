#include "CalendarTimeLineWidget.h"
#include "ui_CalendarTimeLineWidget.h"

CalendarTimeLineWidget::CalendarTimeLineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarTimeLineWidget)
{
    ui->setupUi(this);
}

CalendarTimeLineWidget::~CalendarTimeLineWidget()
{
    delete ui;
}
