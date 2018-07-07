#ifndef CALENDARCELLWIDGET_H
#define CALENDARCELLWIDGET_H

#include <QWidget>
#include <QDate>

namespace Ui {
class CalendarCellWidget;
}

class CalendarCellWidget : public QWidget {
Q_OBJECT

public:
    explicit CalendarCellWidget(QWidget *parent = 0);

    const QDate &getDate() const;

    void setDate(const QDate &date);

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    ~CalendarCellWidget();

private:
    Ui::CalendarCellWidget *ui;
    QDate date;
};

#endif // CALENDARCELLWIDGET_H
