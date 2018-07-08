#ifndef CALENDARMODEWIDGET_H
#define CALENDARMODEWIDGET_H

#include <QWidget>

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

public slots:
    void targetMonth_changes();

private:
    Ui::CalendarModeWidget *ui;

private slots:
    void prevNextButton_pressed();
};

#endif // CALENDARMODEWIDGET_H
