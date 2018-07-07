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
    this->setMinimumSize({200, 200});
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
    QSize dayNumRectSize(30, 30);
    QRect dayNumRect(event->rect().left() + dayNumMargin.left(),
                     event->rect().top() + dayNumMargin.top(),
                     dayNumRectSize.width(),
                     dayNumRectSize.height());
    QFont dayNumFont("Aria", 22);
    painter.setPen(QColor("#71a8e7"));
    painter.setFont(dayNumFont);
    painter.drawText(dayNumRect, Qt::AlignLeft | Qt::AlignVCenter, QString::number(dayNum));

    // draw task
    QMargins taskMargin(5, 5, 5, 5);
    QRect taskRect(dayNumRect.bottomRight() + QPoint(dayNumMargin.right(), dayNumMargin.bottom()),
                   QSize(event->rect().width() - dayNumMargin.left() - dayNumMargin.right() - dayNumRectSize.width() - taskMargin.left() - taskMargin.right(),
                         40));
    todo::DrawUtils::drawRectWithCircle(painter, QFont("Aria", 12), Qt::blue, "Test", taskRect, Qt::green, 0.8);
}

const QDate &CalendarCellWidget::getDate() const {
    return date;
}

void CalendarCellWidget::setDate(const QDate &date) {
    CalendarCellWidget::date = date;
}
