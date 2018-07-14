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
    void refresh_current_items();

signals:
    void itemClicked(const todo::ItemDetail &item);
    void targetDayClicked(const QDate &targetDay);

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

    void setWeekLabelAccordingToWeekWidget();

private slots:
    void prevNextButton_pressed();
    void monthToolButton_pressed();
    void weekToolButton_pressed();

    void prevWeekButton_pressed();
    void nextWeekButton_pressed();

    /**
     * CalendarCellWidget will emit targetDayClicked signal when click on it, but not on the task item.
     * @param targetDay
     */
    void targetDay_clicked(const QDate &targetDay);

    /**
     * Emitted by CalendarWeekWidget. This will cause jumping to daily view.
     * @param targetDay
     */
    void week_targetDay_clicked(const QDate &targetDay);
};

#endif // CALENDARMODEWIDGET_H
