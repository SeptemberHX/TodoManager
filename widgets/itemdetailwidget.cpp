#include "itemdetailwidget.h"
#include "ui_itemdetailwidget.h"
#include "logger.h"
#include <QDebug>
#include <QGlobalStatic>
#include <QPushButton>
#include <QToolButton>
#include <QTimeEdit>
#include <QMessageBox>
#include "itemaddtagwidget.h"
#include "../utils/ItemTagUtils.h"
#include "../utils/itemdetailutils.h"

ItemDetailWidget::ItemDetailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemDetailWidget),
    currItemPtr(nullptr),
    editMode(ItemDetailWidgetMode::VIEW)
{
    ui->setupUi(this);

    this->itemModeBtnGroup = new QButtonGroup(this);
    this->itemModeBtnGroup->addButton(ui->recursionModeButton);
    this->itemModeBtnGroup->addButton(ui->scheduleModeButton);
    this->itemModeBtnGroup->addButton(ui->simpleModeButton);

    // connect(this->itemModeBtnGroup, static_cast<void (QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked), this, &ItemDetailWidget::modeButtonToggled);
    connect(this->itemModeBtnGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &ItemDetailWidget::modeButtonToggled);
    connect(ui->editToolButton, &QToolButton::clicked, this, &ItemDetailWidget::editBtn_clicked);

    connect(ui->scheduleFromTimeEdit, &QTimeEdit::timeChanged, this, &ItemDetailWidget::scheduleTime_changed);
    connect(ui->scheduleToTimeEdit, &QTimeEdit::timeChanged, this, &ItemDetailWidget::scheduleTime_changed);

    // init tags part
    this->tagWidget = new TagWidget(this);
    ui->tagInfoWidgetLayout->addWidget(tagWidget);
    ui->tagLabel->setFixedWidth(ui->tagLabel->fontMetrics().width(ui->tagLabel->text()) + 6);
    // ------ end

    // listWidget change info view when detail edited
    connect(ui->titleLineEdit, &QLineEdit::textChanged, this, &ItemDetailWidget::uiItemEdited);
    connect(ui->descriptionTextEdit, &QTextEdit::textChanged, this, &ItemDetailWidget::uiItemEdited);
    connect(ui->prioritySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ItemDetailWidget::uiItemEdited);
    connect(ui->scheduleFromTimeEdit, &QTimeEdit::timeChanged, this, &ItemDetailWidget::uiItemEdited);
    connect(ui->scheduleToTimeEdit, &QTimeEdit::timeChanged, this, &ItemDetailWidget::uiItemEdited);
    connect(this->tagWidget, &TagWidget::tag_changed, this, &ItemDetailWidget::uiItemEdited);
    // ------ end

    // save or cancel
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &ItemDetailWidget::buttonBox_clicked);

    // finish the task
    connect(ui->finishToolButton, &QToolButton::clicked, this, &ItemDetailWidget::markDoneBtn_clicked);

    // delete the task
    connect(ui->deleteToolButton, &QToolButton::clicked, this, &ItemDetailWidget::deleteButton_clicked);

    // jump to project
    connect(ui->rootProjectLabel, &QLabel::linkActivated, this, &ItemDetailWidget::try_jump_to);
    connect(ui->directProjectLabel, &QLabel::linkActivated, this, &ItemDetailWidget::try_jump_to);

    // set icons
    ui->editToolButton->setIcon(QIcon::fromTheme("edit"));
    ui->deleteToolButton->setIcon(QIcon::fromTheme("editdelete"));
    ui->finishToolButton->setIcon(QIcon::fromTheme("checkmark"));

    this->changeToViewMode();

    // default is disabled. Will change to enabled when loading item.
    this->setEnabled(false);
}

ItemDetailWidget::~ItemDetailWidget()
{
    delete this->currItemPtr;
    delete ui;
}

void ItemDetailWidget::modeButtonToggled() {
    QAbstractButton *currModeButton = this->itemModeBtnGroup->checkedButton();
    Logger::getInstance()->log(currModeButton->objectName() + " pressed");
    if (currModeButton->objectName() == ui->simpleModeButton->objectName()) {
        this->changeToSimpleMode();
    } else if (currModeButton->objectName() == ui->scheduleModeButton->objectName()) {
        this->changeToScheduleMode();
    } else if (currModeButton->objectName() == ui->recursionModeButton->objectName()) {
        this->changeToRecursionMode();
    }
}

void ItemDetailWidget::changeToSimpleMode() {
    this->widgetMode = todo::ItemMode::SIMPLE;

    ui->scheduleFreqWidget->hide();
    ui->scheduleTimeGroupBox->hide();
}

void ItemDetailWidget::changeToScheduleMode() {
    this->widgetMode = todo::ItemMode::SCHEDULE;

    ui->scheduleFreqWidget->hide();
    ui->scheduleTimeGroupBox->show();
}

void ItemDetailWidget::changeToRecursionMode() {
    this->widgetMode = todo::ItemMode::RECURSION;

    ui->scheduleFreqWidget->show();
    ui->scheduleTimeGroupBox->show();
}

void ItemDetailWidget::changeToEditMode() {
    this->editMode = ItemDetailWidgetMode::EDIT;
    this->setReadOnly(false);
}

void ItemDetailWidget::changeToViewMode() {
    this->editMode = ItemDetailWidgetMode::VIEW;
    this->setReadOnly(true);
}

void ItemDetailWidget::setReadOnly(bool isReadOnly) {
    if (isReadOnly) {
        ui->toolButtonWidget->show();
        ui->modeButtonWidget->hide();
        ui->buttonBox->hide();
    } else {
        ui->toolButtonWidget->hide();
        ui->modeButtonWidget->show();
        ui->buttonBox->show();
    }
    this->tagWidget->setReadOnly(isReadOnly);
    if (isReadOnly && this->currItemPtr != nullptr && this->currItemPtr->getTags().size() == 0) {
        ui->tagsWidget->hide();
    } else {
        ui->tagsWidget->show();
    }

    ui->titleLineEdit->setReadOnly(isReadOnly);
    ui->scheduleDateEdit->setReadOnly(isReadOnly);
    ui->scheduleFreqSpinBox->setReadOnly(isReadOnly);
    ui->scheduleFromTimeEdit->setReadOnly(isReadOnly);
    ui->scheduleToTimeEdit->setReadOnly(isReadOnly);

    ui->prioritySpinBox->setReadOnly(isReadOnly);
    ui->scheduleTimeGroupBox->setDisabled(isReadOnly);

    ui->descriptionTextEdit->setReadOnly(isReadOnly);

    if (isReadOnly && this->currItemPtr != nullptr && this->currItemPtr->hasRootGroup()) {
        ui->projectInfoWidget->show();
    } else {
        ui->projectInfoWidget->hide();
    }
}

void ItemDetailWidget::editBtn_clicked() {
    this->changeToEditMode();
}

void ItemDetailWidget::loadItemDetail(const todo::ItemDetail &itemDetail) {
    qDebug() << "Loading " << itemDetail.getTitle();
    if (this->currItemPtr != nullptr) {
        delete this->currItemPtr;
    }

    this->currItemPtr = new todo::ItemDetail(itemDetail);
    this->changeToViewMode();
    this->reloadCurrItemDetail();
    this->setEnabled(true);
}

void ItemDetailWidget::addTagButton(const todo::ItemTag &tag) {
    this->tagWidget->addTag(tag);
}

todo::ItemDetail ItemDetailWidget::collectItemDetail() {
    QList<todo::ItemTag> tagSet;
    for (auto itemTag : this->tagWidget->getTags()) {
        tagSet.append(itemTag);
    }

    todo::ItemDetail result(ui->titleLineEdit->text());
    result.setDescription(ui->descriptionTextEdit->toPlainText());
    result.setMode(this->widgetMode);
    result.setPriority(ui->prioritySpinBox->value());
    result.setTargetDate(ui->scheduleDateEdit->date());
    result.setFromTime(ui->scheduleFromTimeEdit->time());
    result.setToTime(ui->scheduleToTimeEdit->time());
    result.setLastUpdatedTime(QDateTime::currentDateTime());  // current time
    result.setTags(tagSet);
    if (this->currItemPtr != nullptr) {  // get id and raw created time
        result.setId(this->currItemPtr->getId());
        result.setCreatedTime(this->currItemPtr->getCreatedTime());
        result.setDone(this->currItemPtr->isDone());
        result.setRootGroupID(this->currItemPtr->getRootGroupID());
        result.setDirectGroupID(this->currItemPtr->getDirectGroupID());
    }

    return result;
}

bool ItemDetailWidget::checkIfInputLegal() {
    if (ui->titleLineEdit->text().size() == 0) {
        return false;
    }

    return true;
}

void ItemDetailWidget::scheduleTime_changed() {
    QTime toTime = ui->scheduleToTimeEdit->time();
    QTime fromTime = ui->scheduleFromTimeEdit->time();

    if (fromTime > toTime) {
        ui->scheduleToTimeEdit->setTime(fromTime);
    }
}

void ItemDetailWidget::cancelBtn_clicked() {
    this->reloadCurrItemDetail();
}

void ItemDetailWidget::uiItemEdited()
{
    if (this->editMode == ItemDetailWidgetMode::EDIT) {
        emit(ui_item_edited(this->collectItemDetail()));
    }
}

void ItemDetailWidget::reloadCurrItemDetail() {
    switch (this->currItemPtr->getMode()) {
        case todo::ItemMode::SCHEDULE:
            this->changeToScheduleMode();
            ui->scheduleModeButton->click();
            break;
        case todo::ItemMode::SIMPLE:
            this->changeToSimpleMode();
            ui->simpleModeButton->click();
            break;
        case todo::ItemMode::RECURSION:
            this->changeToRecursionMode();
            ui->recursionModeButton->click();
            break;
    }

    ui->titleLineEdit->setText(this->currItemPtr->getTitle());
    this->tagWidget->clear();
    for (auto const &tag : this->currItemPtr->getTags()) {
        this->addTagButton(tag);
    }
    ui->descriptionTextEdit->setText(this->currItemPtr->getDescription());
    ui->scheduleDateEdit->setDate(this->currItemPtr->getTargetDate());
    ui->scheduleFromTimeEdit->setTime(this->currItemPtr->getFromTime());
    ui->scheduleToTimeEdit->setTime(this->currItemPtr->getToTime());
    ui->prioritySpinBox->setValue(this->currItemPtr->getPriority());

    QString info = QString("CreatedTime:\n%1\n\n"
                           "LastUpdatedTime:\n%2").arg(
                                   this->currItemPtr->getCreatedTime().toString("yyyy/MM/dd hh:mm:ss")
                                                  ).arg(
                                   this->currItemPtr->getLastUpdatedTime().toString("yyyy/MM/dd hh:mm:ss")
                                   );
    ui->itemDetailInfoLabel->setText(info);

    if (this->currItemPtr->hasRootGroup()) {
        auto rootGroup = this->dataCenter.selectItemGroupByID(this->currItemPtr->getRootGroupID());
        auto directGroup = this->dataCenter.selectItemGroupByID(this->currItemPtr->getDirectGroupID());
        ui->rootProjectLabel->setText(QString("<a href = %1>%2</a>").arg(rootGroup[0].getId()).arg(rootGroup[0].getTitle()));
        ui->directProjectLabel->setText(QString("<a href = %1>%2</a>").arg(directGroup[0].getId()).arg(directGroup[0].getTitle()));
        ui->projectInfoWidget->show();
    } else {
        ui->projectInfoWidget->hide();
    }
}

void ItemDetailWidget::saveBtn_clicked() {
    if (this->checkIfInputLegal()) {
        emit(saveButtonClicked(this->collectItemDetail()));
    }
}

void ItemDetailWidget::buttonBox_clicked(QAbstractButton *btn) {
    if (dynamic_cast<QPushButton*>(btn) == ui->buttonBox->button(QDialogButtonBox::Save)) {
        this->saveBtn_clicked();
    } else {
        this->cancelBtn_clicked();
    }

    this->changeToViewMode();
}

void ItemDetailWidget::markDoneBtn_clicked() {
    this->currItemPtr->setDone(!this->currItemPtr->isDone());
    emit(markDoneClicked(this->currItemPtr->isDone()));
    // todo: change view to done mode
    // do not use reloadCurrItem(). It will emit currItemEdited signal
    // in other words, mark done is not part of the task's content.
    // use specific signal to handle it.
}

void ItemDetailWidget::deleteButton_clicked() {
    emit(deleteButtonClicked());
}

bool ItemDetailWidget::isEditing() const {
    return this->editMode == ItemDetailWidgetMode::EDIT;
}

void ItemDetailWidget::try_jump_to(const QString &itemID) {
    emit jumpTo(itemID);
}
