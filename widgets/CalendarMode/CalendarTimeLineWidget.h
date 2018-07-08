#ifndef CALENDARTIMELINEWIDGET_H
#define CALENDARTIMELINEWIDGET_H

#include <QWidget>

namespace Ui {
class CalendarTimeLineWidget;
}

class CalendarTimeLineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarTimeLineWidget(QWidget *parent = 0);
    ~CalendarTimeLineWidget();

private:
    Ui::CalendarTimeLineWidget *ui;
};

#endif // CALENDARTIMELINEWIDGET_H
