#include "CalendarTimeLineWidget.h"
#include "ui_CalendarTimeLineWidget.h"
#include <QPainter>
#include <QMargins>
#include <QPaintEvent>
#include <QDebug>
#include <QToolTip>
#include "../../utils/StringUtils.h"
#include "../../utils/ItemUtils.h"

bool compareFun(const todo::ItemDetail &item1, const todo::ItemDetail &item2) {
    return item1.getFromTime() < item2.getFromTime();
}

CalendarTimeLineWidget::CalendarTimeLineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarTimeLineWidget)
{
    ui->setupUi(this);
    this->setMinimumHeight(1000);
    this->setMouseTracking(true);
}

CalendarTimeLineWidget::~CalendarTimeLineWidget()
{
    delete ui;
}

void CalendarTimeLineWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QMargins timeLineMargin(5, 10, 5, 5);
    QPainter painter(this);

    // draw background
    painter.fillRect(event->rect(), Qt::white);

    // calculate echo rect's height and width
    // from 0 to 24, 25 rect in total
    int rectHeight = (this->height() - timeLineMargin.top() - timeLineMargin.bottom()) / 25;
    int rectWidget = (this->width() - timeLineMargin.left() - timeLineMargin.right());
    QRect lastRect(event->rect().left() + timeLineMargin.left(),
                   event->rect().top() + timeLineMargin.top(),
                   rectWidget,
                   rectHeight);
    int hour = event->rect().y() * 26 / this->height();
    QSize defaultHourSize(30, 30);
    QRect lastHourRect(lastRect.left(),
                       lastRect.bottom() - defaultHourSize.height(),
                       defaultHourSize.width(),
                       defaultHourSize.height());
    hour2Rect.clear();
    while (hour <= 24) {
        // draw everything for each hour rect
        painter.setFont(QFont("Aria", 12));
        painter.setPen(QColor("#71a8e7"));
        painter.drawText(lastHourRect, Qt::AlignBottom | Qt::AlignHCenter, QString::number(hour));
        painter.drawLine(lastRect.bottomLeft(), lastRect.bottomRight());
        hour2Rect.insert(hour, lastRect);

        // move to next rect
        lastRect.moveTop(lastRect.bottom());
        lastHourRect.moveBottom(lastRect.bottom());
        ++hour;
    }

    // draw item detail according to their fromTime and endTime
    // remember to check ItemDetail::type
    painter.setPen(Qt::white);
    painter.setFont(QFont("Aria", 8));
    this->itemDetailID2Rect.clear();
    int otherModeCount = 0;
    // draw schedule mode tasks
    foreach(auto itemDetail, this->itemDetailList) {
        if (itemDetail.getMode() == todo::ItemMode::SCHEDULE) {
            auto hRect = this->hour2Rect[itemDetail.getFromTime().hour() + 1];
            int targetTop = hRect.top() + itemDetail.getFromTime().minute() * hRect.height() / 60;
            auto tRect = this->hour2Rect[itemDetail.getToTime().hour() + 1];
            int targetBottom = tRect.top() + itemDetail.getToTime().minute() * tRect.height() / 60;
            QRect targetRect(lastHourRect.right() + 5, targetTop,
                             rectWidget - 35, targetBottom - targetTop);

            QColor targetBgColor(Qt::gray);
            if (!itemDetail.getTags().isEmpty()) {
                targetBgColor = itemDetail.getTags()[0].getColor();
            }
            painter.fillRect(targetRect, targetBgColor);
            QString str = todo::StringUtils::elideText(itemDetail.getTitle(), painter.fontMetrics(), targetRect.width());
            painter.drawText(targetRect, Qt::AlignCenter, str);
            this->itemDetailID2Rect.insert(itemDetail.getId(), targetRect);
        } else {
            ++otherModeCount;
        }
    }

    // draw rect for other type tasks
    if (otherModeCount > 0) {
        QRect targetRect(lastHourRect.right() + 5, hour2Rect[0].top(), rectWidget - 35, rectHeight - 10);
        painter.fillRect(targetRect, Qt::gray);
        QString str = todo::StringUtils::elideText(QString("%1 other tasks").arg(otherModeCount),
                painter.fontMetrics(), targetRect.width());
        painter.drawText(targetRect, Qt::AlignCenter, str);
    }

    // draw mouse hover box
    if (this->mouseHoverPair.first) {
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(this->mouseHoverPair.second);
    }
}

const QList<todo::ItemDetail> &CalendarTimeLineWidget::getItemDetailList() const {
    return itemDetailList;
}

void CalendarTimeLineWidget::setItemDetailList(const QList<todo::ItemDetail> &itemDetailList) {
    CalendarTimeLineWidget::itemDetailList = itemDetailList;
    // sort itemDetailList according to their startDate
    std::sort(this->itemDetailList.begin(), this->itemDetailList.end(), compareFun);
}

void CalendarTimeLineWidget::mouseMoveEvent(QMouseEvent *event) {
    bool lastMouseHoverFlag = this->mouseHoverPair.first;
    this->mouseHoverPair = QPair<bool, QRect>(false, QRect());
    foreach(auto itemDetailID, this->itemDetailID2Rect.keys()) {
        if (this->itemDetailID2Rect[itemDetailID].contains(event->pos())) {
            this->mouseHoverPair = QPair<bool, QRect>(true, this->itemDetailID2Rect[itemDetailID]);

            QString toolTipText;
            foreach(auto itemDetail, this->itemDetailList) {
                if (itemDetail.getId() == itemDetailID) {
                    toolTipText = todo::ItemUtils::generateToolTip(itemDetail);
                    break;
                }
            }
            QToolTip::showText(mapToGlobal(this->itemDetailID2Rect[itemDetailID].bottomRight()), toolTipText);
            break;
        }
    }

    if (!this->mouseHoverPair.first) {
        QToolTip::hideText();
    }
    if (lastMouseHoverFlag != this->mouseHoverPair.first) {
        this->repaint();
    }
    QWidget::mouseMoveEvent(event);
}

void CalendarTimeLineWidget::mousePressEvent(QMouseEvent *event) {
    bool ifItemClicked = false;
    foreach(auto itemDetailID, this->itemDetailID2Rect.keys()) {
        if (this->itemDetailID2Rect[itemDetailID].contains(event->pos())) {
            ifItemClicked = true;
            foreach(auto itemDetail, this->itemDetailList) {
                if (itemDetail.getId() == itemDetailID) {
                    emit itemClicked(itemDetail);
                    break;
                }
            }
        }
    }

    if (ifItemClicked == false) {
        emit targetDayClicked(this->currentDate);
    }
    QWidget::mousePressEvent(event);
}

void CalendarTimeLineWidget::loadDayData(const QDate &targetDay) {
    this->currentDate = targetDay;
    this->setItemDetailList(this->dataCenter.selectItemDetailByDate(targetDay));
    this->repaint();
}
