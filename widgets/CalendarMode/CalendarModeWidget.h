#ifndef CALENDARMODEWIDGET_H
#define CALENDARMODEWIDGET_H

#include <QWidget>
#include "../../data/DataCenter.h"

namespace Ui {
class CalendarModeWidget;
}

class CalendarModeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarModeWidget(QWidget *parent = 0);
    ~CalendarModeWidget();
    void loadMonthData(int year, int month);

signals:
    void itemClicked(const todo::ItemDetail &item);

public slots:
    void targetMonth_changes();
    void item_clicked(const todo::ItemDetail &item);

private:
    Ui::CalendarModeWidget *ui;
    todo::DataCenter dataCenter;

private slots:
    void prevNextButton_pressed();
};

#endif // CALENDARMODEWIDGET_H
