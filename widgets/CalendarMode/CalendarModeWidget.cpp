#include "CalendarModeWidget.h"
#include "ui_CalendarModeWidget.h"
#include "CalendarCellWidget.h"
#include <QDebug>
#include <QLabel>

CalendarModeWidget::CalendarModeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarModeWidget)
{
    ui->setupUi(this);

    this->monthWidget = new CalendarMonthWidget(this);
    this->weekWidget = new CalendarWeekWidget(this);
    ui->mainStackedWidget->addWidget(this->monthWidget);
    ui->mainStackedWidget->addWidget(this->weekWidget);
    ui->mainStackedWidget->setCurrentWidget(this->monthWidget);

    this->toolButtonGroup = new QButtonGroup(this);
    this->toolButtonGroup->addButton(ui->monthToolButton);
    this->toolButtonGroup->addButton(ui->weekToolButton);

    connect(ui->yearSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &CalendarModeWidget::targetMonth_changes);
    connect(ui->monthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &CalendarModeWidget::targetMonth_changes);

    connect(ui->prevToolButton, &QToolButton::clicked, this, &CalendarModeWidget::prevNextButton_pressed);
    connect(ui->nextToolButton, &QToolButton::clicked, this, &CalendarModeWidget::prevNextButton_pressed);

    connect(ui->monthToolButton, &QToolButton::clicked, this, &CalendarModeWidget::monthToolButton_pressed);
    connect(ui->weekToolButton, &QToolButton::clicked, this, &CalendarModeWidget::weekToolButton_pressed);

    connect(this->monthWidget, &CalendarMonthWidget::itemClicked, this, &CalendarModeWidget::item_clicked);

    ui->yearSpinBox->setValue(QDate::currentDate().year());
    ui->monthSpinBox->setValue(QDate::currentDate().month());
}

CalendarModeWidget::~CalendarModeWidget()
{
    delete ui;
}

void CalendarModeWidget::targetMonth_changes() {
    this->monthWidget->loadMonthData(ui->yearSpinBox->value(), ui->monthSpinBox->value());
}

void CalendarModeWidget::prevNextButton_pressed() {
    auto senderBtnPtr = dynamic_cast<QToolButton*>(sender());
    int monthBias = 1;
    if (senderBtnPtr == ui->prevToolButton) {
        monthBias = -1;
    }

    QDate currDate(ui->yearSpinBox->value(), ui->monthSpinBox->value(), 1);
    currDate = currDate.addMonths(monthBias);
    ui->yearSpinBox->setValue(currDate.year());
    ui->monthSpinBox->setValue(currDate.month());
}

void CalendarModeWidget::item_clicked(const todo::ItemDetail &item) {
    emit itemClicked(item);
}

void CalendarModeWidget::monthToolButton_pressed() {
    if (ui->mainStackedWidget->currentWidget() == this->monthWidget) {
        return;
    }

    ui->mainStackedWidget->setCurrentWidget(this->monthWidget);
}

void CalendarModeWidget::weekToolButton_pressed() {
    if (ui->mainStackedWidget->currentWidget() == this->weekWidget) {
        return;
    }

    ui->mainStackedWidget->setCurrentWidget(this->weekWidget);
}
