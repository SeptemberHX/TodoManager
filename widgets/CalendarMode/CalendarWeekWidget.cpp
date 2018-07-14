#include "CalendarWeekWidget.h"
#include "ui_CalendarWeekWidget.h"
#include <QLabel>
#include <QDebug>

CalendarWeekWidget::CalendarWeekWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarWeekWidget)
{
    ui->setupUi(this);

    int weekDay = 7;
    for (int column = 0; column < 7; ++column) {
        if (weekDay > 7) {
            weekDay %= 7;
        }

        // header
        QLabel *tagLabel = new QLabel(QDate::longDayName(weekDay), this);
        tagLabel->setAlignment(Qt::AlignCenter);
        tagLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        ui->gridLayout->addWidget(tagLabel, 0, column);
        ++weekDay;

        // timeline widget
        CalendarTimeLineWidget *timeLineWidget = new CalendarTimeLineWidget(this);
        connect(timeLineWidget, &CalendarTimeLineWidget::itemClicked, this, &CalendarWeekWidget::item_clicked);
        connect(timeLineWidget, &CalendarTimeLineWidget::targetDayClicked, this, &CalendarWeekWidget::targetDay_clicked);
        this->timeLineWidgetList.append(timeLineWidget);
        ui->gridLayout->addWidget(timeLineWidget, 1, column);
    }

    // load this week's data
    this->loadWeekData(QDate::currentDate());
}

CalendarWeekWidget::~CalendarWeekWidget()
{
    delete ui;
}

void CalendarWeekWidget::loadWeekData(const QDate &targetDay) {
    int dayInterval = 0;
    if (targetDay.dayOfWeek() != 7) {  // start from saturday
        dayInterval = targetDay.dayOfWeek();
    }
    this->loadDataFromSaturday(targetDay.addDays(-dayInterval));
}

const QDate &CalendarWeekWidget::getFromDate() const {
    return fromDate;
}

void CalendarWeekWidget::setFromDate(const QDate &fromDate) {
    CalendarWeekWidget::fromDate = fromDate;
}

const QDate &CalendarWeekWidget::getToDate() const {
    return toDate;
}

void CalendarWeekWidget::setToDate(const QDate &toDate) {
    CalendarWeekWidget::toDate = toDate;
}

void CalendarWeekWidget::loadPrevWeek() {
    this->loadDataFromSaturday(this->fromDate.addDays(-7));
}

void CalendarWeekWidget::loadDataFromSaturday(const QDate &firstDayOfWeekInCalendar) {
    for (int i = 0; i < 7; ++i) {
        this->timeLineWidgetList[i]->loadDayData(firstDayOfWeekInCalendar.addDays(i));
    }

    this->fromDate = firstDayOfWeekInCalendar;
    this->toDate = firstDayOfWeekInCalendar.addDays(6);
}

void CalendarWeekWidget::loadNextWeek() {
    this->loadDataFromSaturday(this->fromDate.addDays(7));
}

void CalendarWeekWidget::item_clicked(const todo::ItemDetail &item) {
    emit itemClicked(item);
}

void CalendarWeekWidget::targetDay_clicked(const QDate &targetDay) {
    emit targetDayClicked(targetDay);
}

void CalendarWeekWidget::refresh_current_items() {
    this->loadDataFromSaturday(this->fromDate);
}
