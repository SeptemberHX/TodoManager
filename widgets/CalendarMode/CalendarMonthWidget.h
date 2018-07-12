#ifndef CALENDARMONTHWIDGET_H
#define CALENDARMONTHWIDGET_H

#include <QWidget>
#include "../../data/DataCenter.h"
#include "../../core/ItemDetail.h"

namespace Ui {
class CalendarMonthWidget;
}

class CalendarMonthWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarMonthWidget(QWidget *parent = 0);
    ~CalendarMonthWidget();
    void loadMonthData(int year, int month);

signals:
    void itemClicked(const todo::ItemDetail &item);

public slots:
    void item_clicked(const todo::ItemDetail &item);

private:
    Ui::CalendarMonthWidget *ui;
    todo::DataCenter dataCenter;
};

#endif // CALENDARMONTHWIDGET_H
