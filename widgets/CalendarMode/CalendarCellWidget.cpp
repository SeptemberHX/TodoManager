#include "CalendarCellWidget.h"
#include "ui_CalendarCellWidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QMargins>
#include <QRect>
#include <QToolTip>
#include <QDebug>
#include "../../utils/DrawUtils.h"
#include "../../utils/ItemUtils.h"

CalendarCellWidget::CalendarCellWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarCellWidget)
{
    ui->setupUi(this);
    this->setMinimumSize(QSize(120, 120));
    this->setDateNumColor(QColor("#71a8e7"));
}

CalendarCellWidget::~CalendarCellWidget()
{
    delete ui;
}

void CalendarCellWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

    // draw background
    painter.fillRect(event->rect(), Qt::white);

    // draw date
    int dayNum = this->date.day();
    QMargins dayNumMargin(5, 5, 5, 5);
    QSize dayNumRectSize(30, 20);
    QRect dayNumRect(event->rect().left() + dayNumMargin.left(),
                     event->rect().top() + dayNumMargin.top(),
                     dayNumRectSize.width(),
                     dayNumRectSize.height());
    QFont dayNumFont("Aria", 12);
    if (this->isEnabled()) {
        painter.setPen(this->getDateNumColor());
    } else {
        painter.setPen(Qt::gray);
    }
    painter.setFont(dayNumFont);
    painter.drawText(dayNumRect, Qt::AlignLeft | Qt::AlignVCenter, QString::number(dayNum));

    if (!this->isEnabled()) {
        return;
    }

    // draw task complete percent
    QMargins percentMargin(5, 5, 20, 5);
    QSize percentSize(event->rect().width() - dayNumMargin.left() - dayNumRect.width()
                        - dayNumMargin.right() - percentMargin.left() - percentMargin.right(),
                      20);
    QRect percentRect(dayNumRect.right() + dayNumMargin.right() + percentMargin.left(),
                      dayNumRect.bottom() - percentSize.height(),
                      percentSize.width(),
                      percentSize.height());
    QFont percentFont("Aria", 8);
    double percent = this->getTaskDonePercent();
    QString percentText = QString("%1% of %2").arg(QString::number(percent * 100, 'g', 4))
                                              .arg(QString::number(this->itemDetailList.count()));
    todo::DrawUtils::drawRectWithCircle(painter, percentFont, Qt::darkGray, percentText, percentRect, QColor("#55ffff"), percent);

    // draw task
    QMargins colorMargin(15, 5, 0, 5);
    QRect projectColorRect(QPoint(event->rect().left() + colorMargin.left(), dayNumRect.bottom() + dayNumMargin.bottom()),
                           QSize(12, 24));
    QMargins taskMargin(5, 5, 15, 5);
    int taskSpacing = 3;
    QRect lastTaskRect(QPoint(taskMargin.left() + projectColorRect.right() + colorMargin.right(), dayNumRect.bottom() + dayNumMargin.bottom()),
                       QSize(event->rect().width() - taskMargin.left() - taskMargin.right()
                                         - colorMargin.left() - colorMargin.right() - projectColorRect.width(),
                             24)
                      );
    this->itemDetailID2Rect.clear();
    foreach (auto const &itemDetail, this->itemDetailList) {
        if (lastTaskRect.bottom() + taskMargin.bottom() <= event->rect().bottom()) {
            QColor projectColor = todo::ItemUtils::getRootGroupColor(itemDetail);
            QRect projectColorRectDraw(projectColorRect.center() + QPoint(-6, -6), QSize(12 ,12));
            QPainterPath path;
            path.moveTo(projectColorRectDraw.center());
            path.arcTo(projectColorRectDraw, 0, 360);
            path.closeSubpath();
            painter.fillPath(path, projectColor);

            QColor itemColor(Qt::gray);
            if (!itemDetail.getTags().isEmpty()) {
                itemColor = itemDetail.getTags()[0].getColor();
            }
            todo::DrawUtils::drawRectWithCircle(painter, QFont("Aria", 8), Qt::white,
                                                itemDetail.getTitle(), lastTaskRect, itemColor, 1);
            this->itemDetailID2Rect.insert(itemDetail.getId(), lastTaskRect);
            lastTaskRect.moveTop(lastTaskRect.bottom() + taskSpacing);
            projectColorRect.moveTop(projectColorRect.bottom() + taskSpacing);
        } else {
            break;
        }
    }

    // draw mouse hover box
    if (this->mouseHoverPair.first) {
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(this->mouseHoverPair.second);
    }
}

double CalendarCellWidget::getTaskDonePercent() {
    if (itemDetailList.empty()) return 0;

    double taskCount = itemDetailList.size();
    int doneCount = 0;
    foreach(auto itemDetail, this->itemDetailList) {
        if (itemDetail.isDone()) {
            ++doneCount;
        }
    }

    return doneCount / taskCount;
}

const QDate &CalendarCellWidget::getDate() const {
    return date;
}

void CalendarCellWidget::setDate(const QDate &date) {
    CalendarCellWidget::date = date;
}

const QList<todo::ItemDetail> &CalendarCellWidget::getItemDetailList() const {
    return itemDetailList;
}

void CalendarCellWidget::setItemDetailList(const QList<todo::ItemDetail> &itemDetailList) {
    CalendarCellWidget::itemDetailList = itemDetailList;
    // remember to reset mouseHoverPair.first to false !!!
    this->mouseHoverPair = QPair<bool, QRect>(false, QRect());
}

const QColor &CalendarCellWidget::getDateNumColor() const {
    return dateNumColor;
}

void CalendarCellWidget::setDateNumColor(const QColor &dateNumColor) {
    CalendarCellWidget::dateNumColor = dateNumColor;
}

void CalendarCellWidget::mousePressEvent(QMouseEvent *event) {
    bool ifItemClicked = false;
    foreach(auto itemDetailID, this->itemDetailID2Rect.keys()) {
        if (this->itemDetailID2Rect[itemDetailID].contains(event->pos())) {
            ifItemClicked = true;
            qDebug() << "Item " << itemDetailID << " clicked";
            foreach(auto itemDetail, this->itemDetailList) {
                if (itemDetail.getId() == itemDetailID) {
                    emit itemClicked(itemDetail);
                    break;
                }
            }
            break;
        }
    }

    if (!ifItemClicked) {
        emit targetDayClicked(this->date);
    }
    QWidget::mousePressEvent(event);
}

void CalendarCellWidget::mouseMoveEvent(QMouseEvent *event) {
    bool lastMouseHoverFlag = this->mouseHoverPair.first;
    this->mouseHoverPair = QPair<bool, QRect>(false, QRect());
    foreach(auto itemDetailID, this->itemDetailID2Rect.keys()) {
        if (this->itemDetailID2Rect[itemDetailID].contains(event->pos())) {
            this->mouseHoverPair = QPair<bool, QRect>(true, this->itemDetailID2Rect[itemDetailID]);

            QString toolTipText("%1\n\n%2");
            foreach(auto itemDetail, this->itemDetailList) {
                if (itemDetail.getId() == itemDetailID) {
                    toolTipText = toolTipText.arg(itemDetail.getTitle()).arg(itemDetail.getDescription());
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
