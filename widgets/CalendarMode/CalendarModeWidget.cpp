#include "CalendarModeWidget.h"
#include "ui_CalendarModeWidget.h"

CalendarModeWidget::CalendarModeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarModeWidget)
{
    ui->setupUi(this);
}

CalendarModeWidget::~CalendarModeWidget()
{
    delete ui;
}
