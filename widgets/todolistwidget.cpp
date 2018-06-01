#include "todolistwidget.h"
#include "ui_todolistwidget.h"
#include "../utils/itemdetailutils.h"
#include "../core/SqlErrorException.h"
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

TodoListWidget::TodoListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TodoListWidget),
    isCurrentItemEdited(false)
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

    ui->filterDateEdit->setDate(QDate::currentDate());
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
    for (auto &itemDetail : itemDetails) {
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
    if (newItemDetail.getTargetDate() == ui->filterDateEdit->date()) {
        this->currItemDetailMap.insert(newItemDetail.getId(), newItemDetail);
    }

    // step 3: load new item to list view if possible
    this->addNewItemDetailToListView(newItemDetail);

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
    if (this->isCurrentItemEdited) {
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
    this->isCurrentItemEdited = false;
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
    this->isCurrentItemEdited = true;
    this->currentItem = currentItemDetail;
}

void TodoListWidget::date_filter_changed() {
    QDate targetDate = ui->filterDateEdit->date();
    auto items = this->dataCenter.selectItemDetailByDate(targetDate);

    // First, save it to this->currentItemDetailMap
    this->currItemDetailMap.clear();
    for (auto &item : items) {
        this->currItemDetailMap.insert(item.getId(), item);
    }
    this->isCurrentItemEdited = false;  // don't forget set isCurrentItemEdited to false !

    // Second, load them to list view.
    this->listWidget->loadItemDetails(items);

    // Third, change status bar info
    this->updateStatusBarInfo();
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
    this->listWidget->refresh_item_info(this->currentItem);
    this->isCurrentItemEdited = false;
    this->updateStatusBarInfo();
}

void TodoListWidget::deleteBtn_clicked() {
    if (QMessageBox::warning(this, tr("Attention !!"), tr("You are trying to delete it. Sure to delete it ?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        this->dataCenter.deleteItemDetailByID(this->currentItem.getId());
        this->currItemDetailMap.remove(this->currentItem.getId());
        this->listWidget->removeItemDetailByID(this->currentItem.getId());
        this->isCurrentItemEdited = false;
        this->updateStatusBarInfo();
    }
}

void TodoListWidget::updateStatusBarInfo() {
    int itemCount = this->currItemDetailMap.size();
    int doneItemCount = 0;
    for (auto &item : this->currItemDetailMap.values()) {
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
    if (curr.getTargetDate() == ui->filterDateEdit->date()) {
        this->currItemDetailMap[curr.getId()] = curr;  // save it to detail map
        this->listWidget->refresh_item_info(curr);  // change list widget detail view
        this->detailWidget->loadItemDetail(curr);  // change detail widget content. It will not change by iteself.
    } else {  // if target date changed, remove it.
        this->currItemDetailMap.remove(curr.getId());
        this->listWidget->removeItemDetailByID(curr.getId());
        // we can't change detail widget here because this->curr not refreshed yet.
        // we need to reload detail widget when list widget send selectedItemChanged signal.
    }
    this->isCurrentItemEdited = false;  // reset the flag
}
