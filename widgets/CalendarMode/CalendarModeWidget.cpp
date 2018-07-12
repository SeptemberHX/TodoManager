#include "CalendarModeWidget.h"
#include "ui_CalendarModeWidget.h"
#include "CalendarCellWidget.h"
#include <QDebug>
#include <QLabel>

CalendarModeWidget::CalendarModeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarModeWidget)
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
            QDate cellDate = QDate::currentDate().addDays(d++);
            if (cellDate.dayOfWeek() == 6 || cellDate.dayOfWeek() == 7) {
                cellWidget->setDateNumColor(QColor("#fb5c5d"));
            }

            cellWidget->setDate(cellDate);
            cellWidget->setMouseTracking(true);
            ui->gridLayout->addWidget(cellWidget, i, j);
            connect(cellWidget, &CalendarCellWidget::itemClicked, this, &CalendarModeWidget::item_clicked);
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

    // 6 rows, 7 columns. The first row is label.
    for (int row = 1; row < 7; ++row) {
        for (int column = 0; column < 7; ++column) {
            auto cellWidget = dynamic_cast<CalendarCellWidget *>(ui->gridLayout->itemAtPosition(row, column)->widget());
            cellWidget->setDate(firstDayInCalendar.addDays((row - 1) * 7 + column));

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

void CalendarModeWidget::item_clicked(const todo::ItemDetail &item) {
    emit itemClicked(item);
}
