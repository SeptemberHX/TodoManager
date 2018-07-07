#ifndef CALENDARCELLWIDGET_H
#define CALENDARCELLWIDGET_H

#include <QWidget>

namespace Ui {
class CalendarCellWidget;
}

class CalendarCellWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarCellWidget(QWidget *parent = 0);
    ~CalendarCellWidget();

private:
    Ui::CalendarCellWidget *ui;
};

#endif // CALENDARCELLWIDGET_H
