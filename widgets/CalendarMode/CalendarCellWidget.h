#ifndef CALENDARCELLWIDGET_H
#define CALENDARCELLWIDGET_H

#include <QWidget>
#include <QDate>
#include <QList>
#include "../../core/ItemDetail.h"

namespace Ui {
class CalendarCellWidget;
}

class CalendarCellWidget : public QWidget {
Q_OBJECT

public:
    explicit CalendarCellWidget(QWidget *parent = 0);

    const QDate &getDate() const;

    void setDate(const QDate &date);

    const QList<todo::ItemDetail> &getItemDetailList() const;

    void setItemDetailList(const QList<todo::ItemDetail> &itemDetailList);

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    ~CalendarCellWidget();

private:
    Ui::CalendarCellWidget *ui;
    QDate date;
    QList<todo::ItemDetail> itemDetailList;
    double getTaskDonePercent();
};

#endif // CALENDARCELLWIDGET_H
