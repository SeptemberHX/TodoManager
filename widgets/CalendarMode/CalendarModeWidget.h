#ifndef CALENDARMODEWIDGET_H
#define CALENDARMODEWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include "../../data/DataCenter.h"
#include "CalendarMonthWidget.h"
#include "CalendarWeekWidget.h"

namespace Ui {
class CalendarModeWidget;
}

class CalendarModeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarModeWidget(QWidget *parent = 0);
    ~CalendarModeWidget();

signals:
    void itemClicked(const todo::ItemDetail &item);

public slots:
    void targetMonth_changes();
    void item_clicked(const todo::ItemDetail &item);

private:
    Ui::CalendarModeWidget *ui;
    todo::DataCenter dataCenter;
    CalendarMonthWidget *monthWidget;
    CalendarWeekWidget *weekWidget;
    QButtonGroup *toolButtonGroup;

    QDate weekViewStartDay;
    QDate weekViewEndDay;

private slots:
    void prevNextButton_pressed();
    void monthToolButton_pressed();
    void weekToolButton_pressed();

    void prevWeekButton_pressed();
    void nextWeekButton_pressed();
};

#endif // CALENDARMODEWIDGET_H
