#include "CalendarCellWidget.h"
#include "ui_CalendarCellWidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QMargins>
#include <QRect>
#include "../../utils/DrawUtils.h"

CalendarCellWidget::CalendarCellWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarCellWidget)
{
    ui->setupUi(this);
    this->setMinimumSize({150, 150});
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
    QMargins dayNumMargin(10, 10, 10, 10);
    QSize dayNumRectSize(50, 30);
    QRect dayNumRect(event->rect().left() + dayNumMargin.left(),
                     event->rect().top() + dayNumMargin.top(),
                     dayNumRectSize.width(),
                     dayNumRectSize.height());
    QFont dayNumFont("Aria", 22);
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
    QMargins percentMargin(5, 5, 5, 5);
    QSize percentSize(80, 20);
    QRect percentRect(dayNumRect.right() + dayNumMargin.right() + percentMargin.left(),
                      dayNumRect.bottom() - percentSize.height(),
                      percentSize.width(),
                      percentSize.height());
    QFont percentFont("Aria", 8);
    double percent = this->getTaskDonePercent();
    QString percentText = QString::number(percent * 100, 'g', 4) + QString("%");
    todo::DrawUtils::drawRectWithCircle(painter, percentFont, QColor("#71a8e7"), percentText, percentRect, Qt::gray, percent);

    // draw task
    QMargins taskMargin(5, 5, 5, 5);
    QRect taskRect(dayNumRect.bottomRight() + QPoint(dayNumMargin.right(), dayNumMargin.bottom()),
                   QSize(event->rect().width() - dayNumMargin.left() - dayNumMargin.right() - dayNumRectSize.width() - taskMargin.left() - taskMargin.right(),
                         24));
    todo::DrawUtils::drawRectWithCircle(painter, QFont("Aria", 10), Qt::blue, "飞流直下三千尺，疑是银河落九天", taskRect, QColor("#fb5c5d"), 1);
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
}

const QColor &CalendarCellWidget::getDateNumColor() const {
    return dateNumColor;
}

void CalendarCellWidget::setDateNumColor(const QColor &dateNumColor) {
    CalendarCellWidget::dateNumColor = dateNumColor;
}
