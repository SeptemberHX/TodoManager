#include "CalendarModeWidget.h"
#include "ui_CalendarModeWidget.h"
#include "CalendarCellWidget.h"
#include "../../utils/StringUtils.h"
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
    connect(this->weekWidget, &CalendarWeekWidget::itemClicked, this, &CalendarModeWidget::item_clicked);

    connect(ui->prevWeekButton, &QToolButton::clicked, this, &CalendarModeWidget::prevWeekButton_pressed);
    connect(ui->nextWeekButton, &QToolButton::clicked, this, &CalendarModeWidget::nextWeekButton_pressed);

    connect(this->monthWidget, &CalendarMonthWidget::targetDayClicked, this, &CalendarModeWidget::targetDay_clicked);
    connect(this->weekWidget, &CalendarWeekWidget::targetDayClicked, this, &CalendarModeWidget::week_targetDay_clicked);

    // set icons
    ui->weekToolButton->setIcon(QIcon::fromTheme("view-calendar-week"));
    ui->monthToolButton->setIcon(QIcon::fromTheme("view-calendar-month"));
    ui->prevToolButton->setIcon(QIcon::fromTheme("previous"));
    ui->prevWeekButton->setIcon(QIcon::fromTheme("previous"));
    ui->nextToolButton->setIcon(QIcon::fromTheme("next"));
    ui->nextWeekButton->setIcon(QIcon::fromTheme("next"));

    ui->yearSpinBox->setValue(QDate::currentDate().year());
    ui->monthSpinBox->setValue(QDate::currentDate().month());
    ui->monthToolButton->click();
    this->setWeekLabelAccordingToWeekWidget();

    this->setObjectName(todo::StringUtils::generateUniqueID("CalendarModeWidget"));
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
    ui->monthOperationWidget->show();
    ui->weekOperationWidget->hide();
}

void CalendarModeWidget::weekToolButton_pressed() {
    if (ui->mainStackedWidget->currentWidget() == this->weekWidget) {
        return;
    }

    ui->mainStackedWidget->setCurrentWidget(this->weekWidget);
    ui->weekOperationWidget->show();
    ui->monthOperationWidget->hide();
}

void CalendarModeWidget::prevWeekButton_pressed() {
    this->weekWidget->loadPrevWeek();
    this->setWeekLabelAccordingToWeekWidget();
}

void CalendarModeWidget::nextWeekButton_pressed() {
    this->weekWidget->loadNextWeek();
    this->setWeekLabelAccordingToWeekWidget();
}

void CalendarModeWidget::targetDay_clicked(const QDate &targetDay) {
    if (targetDay.dayOfWeek() == 7) {
        this->weekWidget->loadDataFromSunday(targetDay);
    } else {
        this->weekWidget->loadWeekData(targetDay);
    }
    ui->weekToolButton->click();
    this->setWeekLabelAccordingToWeekWidget();
}

void CalendarModeWidget::setWeekLabelAccordingToWeekWidget() {
    ui->weekLabel->setText(QString(" %1 - %2 ").arg(this->weekWidget->getFromDate().toString("yyyy/MM/dd"))
                                             .arg(this->weekWidget->getToDate().toString("yyyy/MM/dd"))
    );
}

void CalendarModeWidget::week_targetDay_clicked(const QDate &targetDay) {
    emit targetDayClicked(targetDay);
}

void CalendarModeWidget::refresh_current_items() {
    this->targetMonth_changes();
    this->weekWidget->refresh_current_items();
}
