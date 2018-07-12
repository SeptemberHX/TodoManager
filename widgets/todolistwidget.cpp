#include "todolistwidget.h"
#include "ui_todolistwidget.h"
#include "../utils/itemdetailutils.h"
#include "../core/SqlErrorException.h"
#include "../data/filters/DateFilter.h"
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

TodoListWidget::TodoListWidget(QWidget *parent, TodoListWidgetMode viewMode) :
    QWidget(parent),
    ui(new Ui::TodoListWidget),
    currentItemEdited(false),
    viewMode(viewMode)
{
    ui->setupUi(this);

    this->mainSplitter = new QSplitter(this);
    this->detailWidget = new ItemDetailWidget(this);

    this->listWidget = new ItemListWidget(this);
    ui->leftVerticalLayout->addWidget(this->listWidget);

    this->infoLabel = new QLabel(this);
    this->infoLabel->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    ui->leftVerticalLayout->addWidget(this->infoLabel);

    this->mainSplitter->addWidget(ui->leftWidget);
    this->mainSplitter->addWidget(this->detailWidget);
    this->mainSplitter->setStretchFactor(0, 1);
    this->mainSplitter->setStretchFactor(1, 2);
    ui->mainHorizontalLayout->addWidget(this->mainSplitter);

    this->newItemInputDialog = new QInputDialog(this);
    this->newItemInputDialog->setWindowTitle(tr("New todo item"));
    this->newItemInputDialog->setLabelText(tr("What do you want to do ?"));
    this->newItemInputDialog->setInputMode(QInputDialog::TextInput);
    this->newItemInputDialog->resize(800, 300);

    connect(ui->todayToolBtn, &QToolButton::clicked, this, &TodoListWidget::todayBtn_clicked);
    connect(ui->addToolBtn, &QToolButton::clicked, this, &TodoListWidget::addBtn_clicked);
    connect(this->listWidget, &ItemListWidget::selectedItemChanged, this, &TodoListWidget::listWidget_selectedItem_changed);

    connect(this->detailWidget, &ItemDetailWidget::ui_item_edited, this, &TodoListWidget::current_item_edited);  // change info in list table when editing

    connect(ui->preDayToolBtn, &QToolButton::clicked, this, &TodoListWidget::preDayBtn_clicked);
    connect(ui->nextDayToolBtn, &QToolButton::clicked, this, &TodoListWidget::nextDayBtn_clicked);
    connect(ui->filterDateEdit, &QDateEdit::dateChanged, this, &TodoListWidget::date_filter_changed);

    // save and cancel
    connect(this->detailWidget, &ItemDetailWidget::saveButtonClicked, this, &TodoListWidget::saveBtn_clicked);
    // end

    // mark donw
    connect(this->detailWidget, &ItemDetailWidget::markDoneClicked, this, &TodoListWidget::markDone_clicked);
    // end

    // delete
    connect(this->detailWidget, &ItemDetailWidget::deleteButtonClicked, this, &TodoListWidget::deleteBtn_clicked);
    // end

    connect(&this->dataCenter, &todo::DataCenter::itemDetailModified, this, &TodoListWidget::database_modified);

    switch (this->viewMode) {
        case TodoListWidgetMode::DAILY:
            this->changeToDailyMode();
            ui->filterDateEdit->setDate(QDate::currentDate());
            break;
        case TodoListWidgetMode::INBOX:
            this->changeToInboxMode();
            this->loadItemsByInboxCondition(this->currentInboxCondition);
            break;
        case TodoListWidgetMode::TAG:
            this->changeToTagMode();
            break;
        default:
            break;
    }
}

TodoListWidget::~TodoListWidget()
{
    delete ui;
}

void TodoListWidget::todayBtn_clicked()
{
// Only for test...
//    auto itemDetail = ItemDetailUtils::createOneItemDetail();
//    this->currItemDetailMap.insert(itemDetail.getId(), itemDetail);
//    this->loadItemDetailToListView(QList<todo::ItemDetail>{itemDetail});
    ui->filterDateEdit->setDate(QDate::currentDate());
}

void TodoListWidget::loadItemDetailToListView(const QList<todo::ItemDetail> &itemDetails) {
    foreach (auto const &itemDetail, itemDetails) {
        this->listWidget->addItemDetail(itemDetail);
    }
}

void TodoListWidget::dealWithNewItemDetail(const todo::ItemDetail &newItemDetail)
{
    // step 1: save new item to database. If possible, save it to current cache
    try {
        this->saveNewItemDetail(newItemDetail);
    } catch (todo::SqlErrorException &e) {
        qDebug() << "SQL Error Exception happens.";
        QMessageBox::critical(this, tr("Save Error !"), tr("SQL Error Exceptions when trying to save it !!"));
        return;  // Jump out if exception happens
    }

    // step 2: save it to currItemDetails if possible
    this->currItemDetailMap[newItemDetail.getId()] = newItemDetail;

    // step 3: load new item to list view if possible
    this->listWidget->addItemDetail(newItemDetail);

    // step 4: change status bar info
    this->updateStatusBarInfo();
}

void TodoListWidget::saveNewItemDetail(const todo::ItemDetail &newItemDetail)
{
    // maybe do some pre jobs before saving in the future.
    this->dataCenter.insertItemDetail(newItemDetail);
}

void TodoListWidget::addNewItemDetailToListView(const todo::ItemDetail &newItemDetail)
{
    if (newItemDetail.getTargetDate() == ui->filterDateEdit->date()) {
        this->listWidget->addItemDetail(newItemDetail);
    }
}

void TodoListWidget::listWidget_selectedItem_changed(const QString &currItemDetailID) {
    // First, handle the item selected before. If it was modified, we should ask users weather save it or not.
    if (this->currentItemEdited) {
        int result = QMessageBox::information(this, tr("Pay attention !"),
                tr("Current Item has been edited, save it or not ?"), QMessageBox::Save | QMessageBox::No);
        if (result == QMessageBox::Save) {
            this->updateItemDetail(this->currentItem);
        } else if (result == QMessageBox::No) {  // remember set the item view in list widget back here.
            this->listWidget->refresh_item_info(this->currItemDetailMap[this->currentItem.getId()]);
        }
    }

    // Second, reset currentItemEdited flag, set 'currentItem' to the new one and load it to detail widget.
    qDebug() << this->currItemDetailMap[currItemDetailID].getTitle();
    this->currentItemEdited = false;
    this->detailWidget->loadItemDetail(this->currItemDetailMap[currItemDetailID]);
    this->currentItem = this->currItemDetailMap[currItemDetailID];
}

void TodoListWidget::addBtn_clicked() {
    this->newItemInputDialog->setTextValue(tr(""));
    int ok = this->newItemInputDialog->exec();
    if (ok) {
        QString title = this->newItemInputDialog->textValue();
        if (title.size() == 0) {
            QMessageBox::information(this, tr("Invalid input !"), tr("Can't leave it empty !!!"));
            return;
        } else {
            todo::ItemDetail newItemDetail(title);
            newItemDetail.setCreatedTime(QDateTime::currentDateTime());
            newItemDetail.setLastUpdatedTime(QDateTime::currentDateTime());
            newItemDetail.setTargetDate(ui->filterDateEdit->date());
            this->dealWithNewItemDetail(newItemDetail);
        }
    }
}

void TodoListWidget::current_item_edited(const todo::ItemDetail &currentItemDetail) {
    this->listWidget->refresh_item_info(currentItemDetail);
    this->currentItemEdited = true;
    this->currentItem = currentItemDetail;
}

void TodoListWidget::date_filter_changed() {
    QDate targetDate = ui->filterDateEdit->date();
    this->loadTargetDateData(targetDate);
}

void TodoListWidget::preDayBtn_clicked() {
    ui->filterDateEdit->setDate(ui->filterDateEdit->date().addDays(-1));
}

void TodoListWidget::nextDayBtn_clicked() {
    ui->filterDateEdit->setDate(ui->filterDateEdit->date().addDays(1));
}

void TodoListWidget::saveBtn_clicked(const todo::ItemDetail &curr) {
    if (this->currItemDetailMap.find(curr.getId()) != this->currItemDetailMap.end()) {
        this->updateItemDetail(curr);
    }
}

void TodoListWidget::markDone_clicked(bool flag) {
    // remember this->currentItem always holds current item
    // And marking done operation will only be available when detail widget is in view mode
    // so there is no need to change detail widget's view mode
    this->dataCenter.updateDoneByID(this->currentItem.getId(), flag);
    this->currItemDetailMap[this->currentItem.getId()].setDone(flag);
    this->currentItem.setDone(flag);
    this->currentItemEdited = false;
    this->listWidget->refresh_or_remove_item_info(this->currentItem);
    this->updateStatusBarInfo();
}

void TodoListWidget::deleteBtn_clicked() {
    if (QMessageBox::warning(this, tr("Attention !!"), tr("You are trying to delete it. Sure to delete it ?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        this->dataCenter.deleteItemDetailByID(this->currentItem.getId());
        this->currItemDetailMap.remove(this->currentItem.getId());
        this->listWidget->removeItemDetailByID(this->currentItem.getId());
        this->currentItemEdited = false;
        this->updateStatusBarInfo();
    }
}

void TodoListWidget::updateStatusBarInfo() {
    int itemCount = this->currItemDetailMap.size();
    int doneItemCount = 0;
    foreach (auto const &item, this->currItemDetailMap.values()) {
        if (item.isDone()) {
            ++doneItemCount;
        }
    }
    QString newInfoStr(" %1 tasks to do / %2 tasks in all");
    this->infoLabel->setText(newInfoStr.arg(itemCount - doneItemCount).arg(itemCount));
}

void TodoListWidget::updateItemDetail(const todo::ItemDetail &itemDetail) {
    todo::ItemDetail curr(itemDetail);
    curr.setLastUpdatedTime(QDateTime::currentDateTime());
    this->dataCenter.updateItemDetailByID(curr.getId(), curr);  // save to database
    this->currItemDetailMap[curr.getId()] = curr;  // save it to detail map
    this->detailWidget->loadItemDetail(curr);  // change detail widget content. It will not change by itself.

    // reset the flag. Should reset it before refresh list widget,
    // or will be disturbed by list widget removing and adding item and emitting selected_item_changed signal.
    // TODO: redesign the code to avoid this !!
    this->currentItemEdited = false;
    this->listWidget->refresh_or_remove_item_info(curr);  // refresh item if meets condition, or remove it.
}

void TodoListWidget::database_modified() {
    emit(this->databaseModified());
}

void TodoListWidget::changeToInboxMode() {
    ui->inboxModeWidget->show();
    ui->dailyModeWidget->hide();
    ui->tagModeWidget->hide();
    this->viewMode = TodoListWidgetMode::INBOX;
}

void TodoListWidget::changeToDailyMode() {
    ui->inboxModeWidget->hide();
    ui->tagModeWidget->hide();
    ui->dailyModeWidget->show();
    this->viewMode = TodoListWidgetMode::DAILY;
}

void TodoListWidget::changeToTagMode() {
    ui->tagModeWidget->show();
    ui->inboxModeWidget->hide();
    ui->dailyModeWidget->hide();
    this->viewMode = TodoListWidgetMode::TAG;
}

void TodoListWidget::inboxFilter_changed(const InboxViewFilterCondition &newCondition) {
    this->currentInboxCondition = newCondition;
    this->loadItemsByInboxCondition(newCondition);
}

void TodoListWidget::loadItemsByInboxCondition(const InboxViewFilterCondition &cond) {
    auto items = this->dataCenter.selectItemDetailByDate(cond.getTargetFromDate(), cond.getTargetToDate());
    this->loadItems(items);
}

void TodoListWidget::loadItems(const QList<todo::ItemDetail> &items) {
    // First, save it to this->currentItemDetailMap
    this->currItemDetailMap.clear();
    foreach (auto const &item, items) {
        this->currItemDetailMap.insert(item.getId(), item);
    }
    this->currentItemEdited = false;  // don't forget set currentItemEdited to false !

    // Second, load them to list view.
    this->listWidget->loadItemDetails(items);

    // Third, change status bar info
    this->updateStatusBarInfo();
}

void TodoListWidget::refresh_current_items() {
    if (this->viewMode == TodoListWidgetMode::DAILY) {
        this->date_filter_changed();
    } else if (this->viewMode == TodoListWidgetMode::INBOX) {
        this->loadItemsByInboxCondition(this->currentInboxCondition);
    }
}

bool TodoListWidget::isCurrentItemEdited() const {
    return currentItemEdited;
}

void TodoListWidget::jump_to_specific_item(const todo::ItemDetail targetItem) {
    // load target date's tasks
    ui->filterDateEdit->setDate(targetItem.getTargetDate());

    // check whether targetItem exists
    if (this->currItemDetailMap.find(targetItem.getId()) == this->currItemDetailMap.end()) {
        return;
    }

    // set list cursor to targetItem
    this->listWidget->setCurrentItemByID(targetItem.getId());
}

void TodoListWidget::loadTargetDateData(const QDate &targetDate) {
    auto items = this->dataCenter.selectItemDetailByDate(targetDate);

    this->listWidget->clearFilters();
    todo::DateFilter dateFilter(targetDate);
    this->listWidget->addFilter<todo::DateFilter>(dateFilter);

    this->loadItems(items);
}

void TodoListWidget::jump_to_specific_day(const QDate &targetDay) {
    ui->filterDateEdit->setDate(targetDay);
}

// --------- InboxViewFilterCondition --------

const QDate &InboxViewFilterCondition::getTargetFromDate() const {
    return targetFromDate;
}

void InboxViewFilterCondition::setTargetFromDate(const QDate &targetFromDate) {
    InboxViewFilterCondition::targetFromDate = targetFromDate;
}

const QDate &InboxViewFilterCondition::getTargetToDate() const {
    return targetToDate;
}

void InboxViewFilterCondition::setTargetToDate(const QDate &targetToDate) {
    InboxViewFilterCondition::targetToDate = targetToDate;
}

bool InboxViewFilterCondition::isShowDoneItems() const {
    return showDoneItems;
}

void InboxViewFilterCondition::setShowDoneItems(bool showDoneItems) {
    InboxViewFilterCondition::showDoneItems = showDoneItems;
}

bool InboxViewFilterCondition::check(const todo::ItemDetail &item) {
    bool flag = true;
    if (item.getTargetDate() < this->targetFromDate || item.getTargetDate() > this->targetToDate) {
        flag = false;
    } else if (item.isDone() != this->showDoneItems) {
        flag = false;
    }
    return flag;
}

InboxViewFilterCondition::InboxViewFilterCondition() {
    this->setTargetFromDate(QDate::currentDate().addDays(-7));
    this->setTargetToDate(QDate::currentDate());
    this->setShowDoneItems(false);
}
