#ifndef CALENDARCELLWIDGET_H
#define CALENDARCELLWIDGET_H

#include <QWidget>
#include <QDate>
#include <QList>
#include <QMap>
#include "../../core/ItemDetail.h"
#include "../../data/DataCenter.h"

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

    const QColor &getDateNumColor() const;

    void setDateNumColor(const QColor &dateNumColor);



signals:
    void itemClicked(const todo::ItemDetail &item);
    void targetDayClicked(const QDate &targetDay);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

public:
    ~CalendarCellWidget();

private:
    Ui::CalendarCellWidget *ui;
    QDate date;
    QList<todo::ItemDetail> itemDetailList;
    QColor dateNumColor;
    QMap<QString, QRect> itemDetailID2Rect;
    QPair<bool, QRect> mouseHoverPair;
    QFont itemCalendarFont;
    int itemRectHeight;

    double getTaskDonePercent();
};

#endif // CALENDARCELLWIDGET_H
