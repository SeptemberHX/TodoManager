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

private:
    Ui::CalendarModeWidget *ui;
};

#endif // CALENDARMODEWIDGET_H
