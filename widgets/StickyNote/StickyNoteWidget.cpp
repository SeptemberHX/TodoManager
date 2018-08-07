#include "StickyNoteWidget.h"
#include "ui_StickyNoteWidget.h"
#include <QPainter>
#include <QtMath>
#include <QGraphicsDropShadowEffect>
#include <QString>

StickyNoteWidget::StickyNoteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StickyNoteWidget)
{
    ui->setupUi(this);
    this->initWidgetStyle();
    connect(ui->closeToolButton, &QToolButton::clicked, this, &StickyNoteWidget::close);
}

StickyNoteWidget::~StickyNoteWidget()
{
    delete ui;
}

void StickyNoteWidget::initWidgetStyle() {
    ui->closeToolButton->setIcon(QIcon::fromTheme("window-close"));
    ui->editToolButton->setIcon(QIcon::fromTheme("edit"));
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setStyleSheet(this->getStyleSheet("#35b9ab", Qt::white));
}

void StickyNoteWidget::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
}

void StickyNoteWidget::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);
}

void StickyNoteWidget::mouseReleaseEvent(QMouseEvent *event) {
    QWidget::mouseReleaseEvent(event);
}

QString StickyNoteWidget::getStyleSheet(const QColor &bgColor, const QColor &fontColor) {
    QString styleSheetStr(R"(
* {
    background-color: %1;
    color: %2;
}
)");
    return styleSheetStr.arg(bgColor.name()).arg(fontColor.name());
}
