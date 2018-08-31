#ifndef CALENDARTIMELINEWIDGET_H
#define CALENDARTIMELINEWIDGET_H

#include <QWidget>
#include <QList>
#include "../../data/DataCenter.h"
#include "../../core/ItemDetail.h"

namespace Ui {
class CalendarTimeLineWidget;
}

class CalendarTimeLineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarTimeLineWidget(QWidget *parent = 0);

    /**
     *  given a date, load tasks of this day.
     * @param targetDay
     */
    void loadDayData(const QDate &targetDay);

signals:
    void itemClicked(const todo::ItemDetail &item);
    void targetDayClicked(const QDate &targetDay);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    ~CalendarTimeLineWidget();

private:
    Ui::CalendarTimeLineWidget *ui;
    QList<todo::ItemDetailTimeDao> timePieceList;
    QMap<QString, todo::ItemDetail> itemDetailMap;

    QMap<int, QRect> hour2Rect;
    QMap<int, QString> rectIndex2itemDetailID;
    QList<QRect> rectList;

    int otherTaskCounts = 0;
    QPair<bool, QRect> mouseHoverPair;
    todo::DataCenter dataCenter;
    QDate currentDate;
};

#endif // CALENDARTIMELINEWIDGET_H
