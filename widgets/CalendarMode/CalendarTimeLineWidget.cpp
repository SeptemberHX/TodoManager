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
    this->rectIndex2itemDetailID.clear();
    this->rectList.clear();
    // draw schedule mode tasks
    foreach(auto timePiece, this->timePieceList) {
        auto hRect = this->hour2Rect[timePiece.getStartTime().time().hour() + 1];
        int targetTop = hRect.top() + timePiece.getStartTime().time().minute() * hRect.height() / 60;
        auto tRect = this->hour2Rect[timePiece.getEndTime().time().hour() + 1];
        int targetBottom = tRect.top() + timePiece.getEndTime().time().minute() * tRect.height() / 60;
        QRect targetRect(lastHourRect.right() + 5, targetTop,
                         rectWidget - 35, targetBottom - targetTop);

        QColor targetBgColor(Qt::gray);
        auto itemDetail = this->itemDetailMap[timePiece.getItemID()];
        if (!itemDetail.getTags().isEmpty()) {
            targetBgColor = this->itemDetailMap[timePiece.getItemID()].getTags()[0].getColor();
        }
        painter.fillRect(targetRect, targetBgColor);
        QString str = todo::StringUtils::elideText(itemDetail.getTitle(), painter.fontMetrics(), targetRect.width());
        painter.drawText(targetRect, Qt::AlignCenter, str);

        this->rectList.append(targetRect);
        this->rectIndex2itemDetailID.insert(this->rectList.size() - 1, itemDetail.getId());
    }

    // draw rect for other type tasks
    if (this->otherTaskCounts > 0) {
        QRect targetRect(lastHourRect.right() + 5, hour2Rect[0].top(), rectWidget - 35, rectHeight - 10);
        painter.fillRect(targetRect, Qt::gray);
        QString str = todo::StringUtils::elideText(QString("%1 other tasks").arg(this->otherTaskCounts),
                painter.fontMetrics(), targetRect.width());
        painter.drawText(targetRect, Qt::AlignCenter, str);
    }

    // draw mouse hover box
    if (this->mouseHoverPair.first) {
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(this->mouseHoverPair.second);
    }
}

void CalendarTimeLineWidget::mouseMoveEvent(QMouseEvent *event) {
    bool lastMouseHoverFlag = this->mouseHoverPair.first;
    this->mouseHoverPair = QPair<bool, QRect>(false, QRect());
    foreach(auto rectIndex, this->rectIndex2itemDetailID.keys()) {
        if (this->rectList[rectIndex].contains(event->pos())) {
            this->mouseHoverPair = QPair<bool, QRect>(true, this->rectList[rectIndex]);

            QString toolTipText = todo::ItemUtils::generateToolTip(
                    this->itemDetailMap[this->rectIndex2itemDetailID[rectIndex]]);
            QToolTip::showText(mapToGlobal(this->rectList[rectIndex].bottomRight()), toolTipText);
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
    foreach(auto rectIndex, this->rectIndex2itemDetailID.keys()) {
        if (this->rectList[rectIndex].contains(event->pos())) {
            ifItemClicked = true;
            emit itemClicked(this->itemDetailMap[this->rectIndex2itemDetailID[rectIndex]]);
        }
    }

    if (ifItemClicked == false) {
        emit targetDayClicked(this->currentDate);
    }
    QWidget::mousePressEvent(event);
}

void CalendarTimeLineWidget::loadDayData(const QDate &targetDay) {
    this->currentDate = targetDay;

    // get all tasks on target day, including tasks which are archived on target day
    auto tasksOnTargetDay = this->dataCenter.selectItemDetailByDate(targetDay);
    auto timePieceOnTargetDay = this->dataCenter.selectItemDetailTimeByTargetDate(targetDay);
    QSet<QString> otherItemIds;
    foreach (auto const &timePiece, timePieceOnTargetDay) {
        otherItemIds.insert(timePiece.getItemID());
    }
    tasksOnTargetDay.append(this->dataCenter.selectItemDetailByIDs(otherItemIds.toList()));

    // record all tasks
    foreach (auto const &itemDetail, tasksOnTargetDay) {
        this->itemDetailMap[itemDetail.getId()] = itemDetail;  // override duplicate
    }
    QDateTime todayTime(targetDay);
    int taskNeedToShowCount = 0;
    foreach (auto const &itemDetail, this->itemDetailMap.values()) {
        foreach (auto const &timePiece, itemDetail.getTimeDaos()) {
            if (timePiece.getEndTime() < todayTime || timePiece.getStartTime() >= todayTime.addDays(1)) continue;
            this->timePieceList.append(timePiece);
            ++taskNeedToShowCount;
        }

        if (itemDetail.getTargetDate() == targetDay
                && itemDetail.getTimeDaos().empty()
                && itemDetail.getMode() == todo::ItemMode::SCHEDULE) {
            todo::ItemDetailTimeDao tmpTimeDao;
            tmpTimeDao.setItemID(itemDetail.getId());
            QDateTime tmpTime(targetDay);
            tmpTime.setTime(itemDetail.getFromTime());
            tmpTimeDao.setStartTime(tmpTime);
            tmpTime.setTime(itemDetail.getToTime());
            tmpTimeDao.setEndTime(tmpTime);
            this->timePieceList.append(tmpTimeDao);
            ++taskNeedToShowCount;
        }
    }
    std::sort(this->timePieceList.begin(), this->timePieceList.end());
    this->otherTaskCounts = this->itemDetailMap.size() - taskNeedToShowCount;

    this->repaint();
}
