#include "CalendarModeWidget.h"
#include "ui_CalendarModeWidget.h"
#include "CalendarCellWidget.h"
#include <QDebug>

CalendarModeWidget::CalendarModeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarModeWidget)
{
    ui->setupUi(this);

    int d = 0;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            auto cellWidget = new CalendarCellWidget(this);
            QDate cellDate = QDate::currentDate().addDays(d++);
            if (cellDate.dayOfWeek() == 6 || cellDate.dayOfWeek() == 7) {
                cellWidget->setDateNumColor(QColor("#fb5c5d"));
            }

            cellWidget->setDate(cellDate);
            ui->gridLayout->addWidget(cellWidget, i, j);
        }
    }

    connect(ui->yearSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &CalendarModeWidget::targetMonth_changes);
    connect(ui->monthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &CalendarModeWidget::targetMonth_changes);

    connect(ui->prevToolButton, &QToolButton::clicked, this, &CalendarModeWidget::prevNextButton_pressed);
    connect(ui->nextToolButton, &QToolButton::clicked, this, &CalendarModeWidget::prevNextButton_pressed);

    ui->yearSpinBox->setValue(QDate::currentDate().year());
    ui->monthSpinBox->setValue(QDate::currentDate().month());
}

CalendarModeWidget::~CalendarModeWidget()
{
    delete ui;
}

void CalendarModeWidget::loadMonthData(int year, int month) {
    QDate firstDayInMonth(year, month, 1);
    int dayInterval = 0;
    if (firstDayInMonth.dayOfWeek() == 7) {  // start from saturday
        dayInterval = 6;
    } else {
        dayInterval = firstDayInMonth.dayOfWeek();
    }
    QDate firstDayInCalendar = firstDayInMonth.addDays(-dayInterval);

    // 5 rows, 7 columns
    for (int row = 0; row < 6; ++row) {
        for (int column = 0; column < 7; ++column) {
            auto cellWidget = dynamic_cast<CalendarCellWidget *>(ui->gridLayout->itemAtPosition(row, column)->widget());
            cellWidget->setDate(firstDayInCalendar.addDays(row * 7 + column));

            // load ItemDetail
            if (cellWidget->getDate().month() != month) {  // jump over days not in target month
                cellWidget->setEnabled(false);
            } else {
                cellWidget->setEnabled(true);
                // load ItemDetail
                cellWidget->setItemDetailList(this->dataCenter.selectItemDetailByDate(cellWidget->getDate()));
            }
            cellWidget->repaint();
        }
    }
}

void CalendarModeWidget::targetMonth_changes() {
    this->loadMonthData(ui->yearSpinBox->value(), ui->monthSpinBox->value());
}

void CalendarModeWidget::prevNextButton_pressed() {
    auto senderBtnPtr = dynamic_cast<QToolButton*>(sender());
    int monthBias = 1;
    if (senderBtnPtr == ui->prevToolButton) {
        monthBias = -1;
    }

    QDate currDate(ui->yearSpinBox->value(), ui->monthSpinBox->value(), 1);
    currDate = currDate.addMonths(monthBias);
    ui->yearSpinBox->setValue(currDate.year());
    ui->monthSpinBox->setValue(currDate.month());
}
