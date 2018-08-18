#include "todolistwidget.h"
#include "ui_todolistwidget.h"
#include "../utils/itemdetailutils.h"
#include "../core/SqlErrorException.h"
#include "../data/filters/DateFilter.h"
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include "Common/CommonAction.h"
#include "../utils/StringUtils.h"

TodoListWidget::TodoListWidget(QWidget *parent, TodoListWidgetMode viewMode) :
    QWidget(parent),
    ui(new Ui::TodoListWidget),
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

    this->itemListMenu = new QMenu(this);

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

    // jump to group
    connect(this->detailWidget, &ItemDetailWidget::jumpTo, this, &TodoListWidget::jump_to_specific_group);
    // end

    // jump to tag
    connect(this->detailWidget, &ItemDetailWidget::tagClicked, this, &TodoListWidget::item_tag_clicked);

    // database modified
    connect(&this->dataCenter, &todo::DataCenter::databaseModified, this, &TodoListWidget::database_modified);

    // list right click menu
    connect(this->listWidget, &ItemListWidget::customContextMenuRequested, this, &TodoListWidget::show_list_content_menu);

    // set icons
    ui->tagSorterToolButton->setIcon(QIcon::fromTheme("sort-presence"));
    ui->preDayToolBtn->setIcon(QIcon::fromTheme("previous"));
    ui->nextDayToolBtn->setIcon(QIcon::fromTheme("next"));
    ui->todayToolBtn->setIcon(QIcon::fromTheme("go-today"));
    ui->addToolBtn->setIcon(QIcon::fromTheme("list-add"));
    ui->inboxAddToolBtn->setIcon(QIcon::fromTheme("list-add"));
    ui->inboxConditionToolBtn->setIcon(QIcon::fromTheme("sort-presence"));

    this->setObjectName(todo::StringUtils::generateUniqueID("TodoListWidget"));

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
    ui->filterDateEdit->setDate(QDate::currentDate());
}

void TodoListWidget::loadItemDetailToListView(const QList<todo::ItemDetail> &itemDetails) {
    foreach (auto const &itemDetail, itemDetails) {
        this->listWidget->addItemWrapper(itemDetail);
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

    // step 2: load new item to list view if possible
    this->listWidget->addItemWrapper(newItemDetail);

    // step 3: change status bar info
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
        this->listWidget->addItemWrapper(newItemDetail);
    }
}

void TodoListWidget::listWidget_selectedItem_changed(const QString &currItemDetailID) {
    // First, handle the item selected before. If it was modified, we should ask users weather save it or not.
    if (this->detailWidget->isEditing()) {
        int result = QMessageBox::information(this, tr("Pay attention !"),
                tr("Current Item has been edited, save it or not ?"), QMessageBox::Save | QMessageBox::No);
        if (result == QMessageBox::Save) {
            this->updateItemDetail(this->detailWidget->collectItemDetail());
        } else if (result == QMessageBox::No) {  // remember set the item view in list widget back here.
            this->listWidget->refresh_item_info(this->dataCenter.selectItemDetailByID(currItemDetailID));
        }
    }

    // Second, set 'currentItem' to the new one and load it to detail widget.
    this->detailWidget->loadItemDetail(this->dataCenter.selectItemDetailByID(currItemDetailID));
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
    this->updateItemDetail(curr);
}

void TodoListWidget::markDone_clicked(bool flag) {
    // remember this->currentItem always holds current item
    // And marking done operation will only be available when detail widget is in view mode
    // so there is no need to change detail widget's view mode
    this->dataCenter.updateDoneByID(this->getCurrentItemID(), flag);
    this->listWidget->refresh_or_remove_item_info(this->dataCenter.selectItemDetailByID(this->getCurrentItemID()));
    this->updateStatusBarInfo();
}

void TodoListWidget::deleteBtn_clicked() {
    if (QMessageBox::warning(this, tr("Attention !!"), tr("You are trying to delete it. Sure to delete it ?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        this->dataCenter.deleteItemDetailByIDCompletely(this->getCurrentItemID());
        this->listWidget->removeItemWrapperByID(this->getCurrentItemID());
        this->updateStatusBarInfo();
    }
}

void TodoListWidget::updateStatusBarInfo() {
    auto itemIDList = this->listWidget->getAllItemIDs();
    int itemCount = itemIDList.size();
    int doneItemCount = 0;
    foreach (auto const &item, this->dataCenter.selectItemDetailByIDs(itemIDList)) {
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
    this->dataCenter.updateItemDetailByID(curr.getId(), this->dataCenter.selectItemDetailByID(curr.getId()), curr);  // save to database
    this->detailWidget->loadItemDetail(curr);  // change detail widget content. It will not change by itself.

    this->listWidget->refresh_or_remove_item_info(curr);  // refresh item if meets condition, or remove it.
}

void TodoListWidget::database_modified() {
    emit(this->databaseModified(this->objectName()));
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
    // First, load them to list view.
    this->listWidget->loadItemDetails(items);

    // Second, change status bar info
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
    return this->detailWidget->isEditing();
}

void TodoListWidget::jump_to_specific_item(const todo::ItemDetail targetItem) {
    // load target date's tasks
    ui->filterDateEdit->setDate(targetItem.getTargetDate());

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

void TodoListWidget::jump_to_specific_group(const QString &groupID) {
    emit jumpToGroup(groupID);
}

bool TodoListWidget::isEditing() const {
    return this->detailWidget->isEditing();
}

void TodoListWidget::item_tag_clicked(const QString &itemID) {
    emit jumpToTag(itemID);
}

void TodoListWidget::rightClickMenu_clicked() {
    switch (CommonAction::getInstance()->getActionType(sender()->objectName())) {
        case CommonActionType::ITEMDETAIL_ASSIGN_TO_PROJECT:
            if (this->isEditing()) {
                QMessageBox::information(this, "Attention!", "Save your work first !!");
                return;
            }
            this->assignItemDetailsToProject(this->listWidget->getSelectedItemIDs());
            break;
        default:
            break;
    }
}

void TodoListWidget::show_list_content_menu(const QPoint &point) {
    if (!this->listWidget->isPointHasItem(point)) return;
    this->initRightClickedMenu(this->listWidget->getSelectedItemIDs().size() > 1);
    this->itemListMenu->popup(QCursor::pos());
}

void TodoListWidget::initRightClickedMenu(bool multiSelected) {
    this->itemListMenu->clear();
    auto actionTypeList = CommonAction::getInstance()->getItemDetailActions(multiSelected);
    foreach (auto const &actionType, actionTypeList) {
        QAction *actionPtr = new QAction(CommonAction::getInstance()->getActionName(actionType), this->itemListMenu);
        actionPtr->setObjectName(actionPtr->text());
        connect(actionPtr, &QAction::triggered, this, &TodoListWidget::rightClickMenu_clicked);
        this->itemListMenu->addAction(actionPtr);
    }
}

void TodoListWidget::assignItemDetailsToProject(const QList<QString> &itemDetailIDList) {
    ProjectChooseDialog projectChooseDialog(this);
    if (projectChooseDialog.exec() != QDialog::Accepted) return;

    auto projectIDPair = projectChooseDialog.getSelectedProjectIDPair();
    auto itemDetails = this->dataCenter.selectItemDetailByIDs(itemDetailIDList);
    QList<todo::ItemDetail> newItemDetailList;
    foreach (auto const &item, itemDetails) {
        todo::ItemDetail itemCopy(item);
        itemCopy.setDirectGroupID(projectIDPair.second);
        itemCopy.setRootGroupID(projectIDPair.first);
        newItemDetailList.append(itemCopy);
    }
    this->dataCenter.updateItemDetailsByIDList(itemDetailIDList, itemDetails, newItemDetailList);
    foreach (auto const &newItem, newItemDetailList) {
        this->listWidget->refresh_item_info(newItem);
    }
}

QString TodoListWidget::getCurrentItemID() {
    return this->listWidget->currentItemID();
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
