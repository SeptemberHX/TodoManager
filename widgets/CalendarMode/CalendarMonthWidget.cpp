#include "CalendarMonthWidget.h"
#include "ui_CalendarMonthWidget.h"
#include <QLabel>
#include <QDate>
#include <QDebug>
#include "CalendarCellWidget.h"

CalendarMonthWidget::CalendarMonthWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarMonthWidget)
{
    ui->setupUi(this);

    int weekDay = 7;
    for (int column = 0; column < 7; ++column) {
        if (weekDay > 7) {
            weekDay %= 7;
        }

        QLabel *tagLabel = new QLabel(QDate::longDayName(weekDay), this);
        tagLabel->setAlignment(Qt::AlignCenter);
        tagLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        ui->gridLayout->addWidget(tagLabel, 0, column);
        ++weekDay;
    }

    int d = 0;
    for (int i = 1; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            auto cellWidget = new CalendarCellWidget(this);
            cellWidget->setMouseTracking(true);
            ui->gridLayout->addWidget(cellWidget, i, j);
            connect(cellWidget, &CalendarCellWidget::itemClicked, this, &CalendarMonthWidget::item_clicked);
            connect(cellWidget, &CalendarCellWidget::targetDayClicked, this, &CalendarMonthWidget::targetDay_cicked);
        }
    }

}

CalendarMonthWidget::~CalendarMonthWidget()
{
    delete ui;
}

void CalendarMonthWidget::loadMonthData(int year, int month) {
    QDate firstDayInMonth(year, month, 1);
    int dayInterval = 0;
    if (firstDayInMonth.dayOfWeek() != 7) {  // start from sunday
        dayInterval = firstDayInMonth.dayOfWeek();
    }
    QDate firstDayInCalendar = firstDayInMonth.addDays(-dayInterval);

    // 6 rows, 7 columns. The first row is label.
    for (int row = 1; row < 7; ++row) {
        for (int column = 0; column < 7; ++column) {
            auto cellWidget = dynamic_cast<CalendarCellWidget *>(ui->gridLayout->itemAtPosition(row, column)->widget());
            auto currDay = firstDayInCalendar.addDays((row - 1) * 7 + column);
            cellWidget->setDate(currDay);
            if (currDay.dayOfWeek() == 6 || currDay.dayOfWeek() == 7) {
                cellWidget->setDateNumColor(QColor("#fb5c5d"));
            }

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

void CalendarMonthWidget::item_clicked(const todo::ItemDetail &item) {
    emit itemClicked(item);
}

void CalendarMonthWidget::targetDay_cicked(const QDate &targetDay) {
    emit targetDayClicked(targetDay);
}
