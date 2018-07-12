#ifndef CALENDARWEEKWIDGET_H
#define CALENDARWEEKWIDGET_H

#include <QWidget>

namespace Ui {
class CalendarWeekWidget;
}

class CalendarWeekWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarWeekWidget(QWidget *parent = 0);
    ~CalendarWeekWidget();

private:
    Ui::CalendarWeekWidget *ui;
};

#endif // CALENDARWEEKWIDGET_H
