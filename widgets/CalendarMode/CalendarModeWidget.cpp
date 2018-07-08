#include "CalendarModeWidget.h"
#include "ui_CalendarModeWidget.h"
#include "CalendarCellWidget.h"

CalendarModeWidget::CalendarModeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarModeWidget)
{
    ui->setupUi(this);

    int d = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 7; ++j) {
            auto cellWidget = new CalendarCellWidget(this);
            cellWidget->setDate(QDate::currentDate().addDays(d++));
            ui->gridLayout->addWidget(cellWidget, i, j);
        }
    }
}

CalendarModeWidget::~CalendarModeWidget()
{
    delete ui;
}
