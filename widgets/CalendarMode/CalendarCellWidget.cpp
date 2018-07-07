#include "CalendarCellWidget.h"
#include "ui_CalendarCellWidget.h"

CalendarCellWidget::CalendarCellWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarCellWidget)
{
    ui->setupUi(this);
}

CalendarCellWidget::~CalendarCellWidget()
{
    delete ui;
};
