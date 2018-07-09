#ifndef CALENDARTIMELINEWIDGET_H
#define CALENDARTIMELINEWIDGET_H

#include <QWidget>
#include <QList>
#include "../../core/ItemDetail.h"

namespace Ui {
class CalendarTimeLineWidget;
}

class CalendarTimeLineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarTimeLineWidget(QWidget *parent = 0);

    const QList<todo::ItemDetail> &getItemDetailList() const;

    void setItemDetailList(const QList<todo::ItemDetail> &itemDetailList);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    ~CalendarTimeLineWidget();

private:
    Ui::CalendarTimeLineWidget *ui;
    QList<todo::ItemDetail> itemDetailList;
    QMap<int, QRect> hour2Rect;
    QMap<QString, QRect> itemDetailID2Rect;
    QPair<bool, QRect> mouseHoverPair;
};

#endif // CALENDARTIMELINEWIDGET_H