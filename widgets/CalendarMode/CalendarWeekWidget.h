#ifndef CALENDARWEEKWIDGET_H
#define CALENDARWEEKWIDGET_H

#include <QWidget>
#include "CalendarTimeLineWidget.h"

namespace Ui {
class CalendarWeekWidget;
}

class CalendarWeekWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarWeekWidget(QWidget *parent = 0);
    ~CalendarWeekWidget();

    /**
     * show the data of the target week which includes given day
     * @param year
     * @param month
     * @param day
     */
    void loadWeekData(const QDate &targetDay);

    const QDate &getFromDate() const;

    void setFromDate(const QDate &fromDate);

    const QDate &getToDate() const;

    void setToDate(const QDate &toDate);

    void loadPrevWeek();

    void loadNextWeek();

signals:
    void itemClicked(const todo::ItemDetail &item);

private:
    Ui::CalendarWeekWidget *ui;
    QList<CalendarTimeLineWidget*> timeLineWidgetList;

    QDate fromDate;
    QDate toDate;

    void loadDataFromSaturday(const QDate &firstDayOfWeekInCalendar);
    void item_clicked(const todo::ItemDetail &item);
};

#endif // CALENDARWEEKWIDGET_H
