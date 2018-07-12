#include "itemlistwidget.h"
#include "ui_itemlistwidget.h"
#include "../utils/itemdetailutils.h"
#include "../data/sorters/SorterOrganize.h"
#include "../data/sorters/DoneSorter.h"
#include "ItemListItemDelegate.h"
#include "../data/sorters/DateSorter.h"
#include <QScrollBar>
#include <QDebug>
#include <QListView>

ItemListWidget::ItemListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemListWidget)
{
    ui->setupUi(this);
    this->listView = new CustomListView(this);
    ui->verticalLayout->addWidget(this->listView);
    this->itemModel = new QStandardItemModel(this->listView);
    this->listView->setModel(this->itemModel);
    this->listView->setItemDelegate(new ItemListItemDelegate(this->listView));
    this->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->listView->setMinimumWidth(430);
//    this->listView->setDragEnabled(true);
//    this->listView->setDragDropMode(QAbstractItemView::DragDrop);

    connect(this->listView, &CustomListView::currentIndexChanged, this, &ItemListWidget::listWidget_selectedItem_changed);

    // ------ default sorter ------
    this->addSorter(todo::DoneSorter(true));
    this->addSorter(todo::DateSorter());
    // ------ end ------
}

ItemListWidget::~ItemListWidget()
{
    delete ui;
    this->clearFilters();
    this->clearSorters();
}

void ItemListWidget::addItemDetail(const todo::ItemDetail &item)
{
    if (this->checkItem(item)) {
        todo::SorterOrganize comparer(this->sorters);
        int newRowToInsert = 0;  // insert to top by default
        for (int i = 0; i < this->itemModel->rowCount(); ++i) {
            todo::ItemDetail rowItem = this->itemModel->item(i)->data(Qt::UserRole + 1).value<todo::ItemDetail>();
            if (!comparer(item, rowItem)) {
                newRowToInsert = i;
                break;
            }
        }

        // insert it
        auto newListItem = new QStandardItem();
        newListItem->setData(QVariant::fromValue(item), Qt::UserRole + 1);
        this->itemModel->insertRow(newRowToInsert, newListItem);
        this->listView->setCurrentIndex(this->itemModel->indexFromItem(newListItem));
    }
}

void ItemListWidget::refresh_item_info(const todo::ItemDetail &item)
{
    int targetRow = this->findRow(item);
    if (targetRow != -1) {
        this->itemModel->item(targetRow)->setData(QVariant::fromValue(item), Qt::UserRole + 1);
    }
}

void ItemListWidget::listWidget_selectedItem_changed() {
    todo::ItemDetail currItemDetail = this->listView->currentIndex().data(Qt::UserRole + 1).value<todo::ItemDetail>();
    emit selectedItemChanged(currItemDetail.getId());
}

void ItemListWidget::loadItemDetails(const QList<todo::ItemDetail> &items) {
    this->itemModel->clear();
    QList<todo::ItemDetail> itemsAfterFilter(items);
    foreach (auto const &filterPtr, this->filters) {
        itemsAfterFilter = filterPtr->filter(itemsAfterFilter);
    }

    itemsAfterFilter = this->sortItemlist(itemsAfterFilter);  // sort it

    // strange here. If we use range-based for loop, then we will get SIGSEGV
    // Due to Qt 5.11 official document, range-based for loop might force a Qt container to detach.
    foreach (auto const &item, itemsAfterFilter) {
        this->addItemDetail_(item);  // Use addItemDetail method to sort new item according to sorters.
    }
}

void ItemListWidget::removeItemDetailByID(const QString &itemID) {
    todo::ItemDetail prevItemDetail = this->listView->currentIndex().data(Qt::UserRole + 1).value<todo::ItemDetail>();
    qDebug() << prevItemDetail.getTitle();
    for (int i = 0; i < this->itemModel->rowCount(); ++i) {
        todo::ItemDetail rowItem = this->itemModel->item(i)->data(Qt::UserRole + 1).value<todo::ItemDetail>();
        if (rowItem.getId() == itemID) {
            this->itemModel->removeRow(i);
            break;
        }
    }
    todo::ItemDetail currItemDetail = this->listView->currentIndex().data(Qt::UserRole + 1).value<todo::ItemDetail>();
    qDebug() << currItemDetail.getTitle();
    if (currItemDetail.getId() != prevItemDetail.getId()) {
        emit selectedItemChanged(currItemDetail.getId());
    }
}

void ItemListWidget::clearFilters() {
    foreach (auto const &ptr, this->filters) {
        delete ptr;
    }
    this->filters.clear();
}

QList<todo::ItemDetail> ItemListWidget::filtItemList(const QList<todo::ItemDetail> &details) {
    QList<todo::ItemDetail> resultLists = details;
    foreach (auto const &filterPtr, this->filters) {
        resultLists = filterPtr->filter(resultLists);
    }
    return resultLists;
}

bool ItemListWidget::checkItem(const todo::ItemDetail &detail) {
    bool passCheckFlag = true;
    foreach (auto const &filterPtr, this->filters) {
        passCheckFlag &= filterPtr->check(detail);
        if (!passCheckFlag) {
            break;
        }
    }
    return passCheckFlag;
}

QList<todo::ItemDetail> ItemListWidget::addItemDetail_(const todo::ItemDetail &item) {
    auto newListItem = new QStandardItem();
    newListItem->setData(QVariant::fromValue(item), Qt::UserRole + 1);
    this->itemModel->insertRow(0, newListItem);
}

void ItemListWidget::refresh_or_remove_item_info(const todo::ItemDetail &item) {
    if (this->checkItem(item)) {
        this->refresh_item_info_and_sort(item);
    } else {
        this->removeItemDetailByID(item.getId());
    }
}

void ItemListWidget::clearSorters() {
    foreach (auto const &ptr, this->sorters) {
        delete ptr;
    }
    this->sorters.clear();
}

QList<todo::ItemDetail> ItemListWidget::sortItemlist(const QList<todo::ItemDetail> &details) {
    QList<todo::ItemDetail> results(details);
    std::sort(results.begin(), results.end(), todo::SorterOrganize(this->sorters));
    return results;
}

void ItemListWidget::refresh_item_info_and_sort(const todo::ItemDetail &item) {
    int targetRow = this->findRow(item);
    if (targetRow < 0) {
        return;
    }

    // remove it first, then compare it to others from top to bottom to find the right place.
    itemModel->removeRow(targetRow);
    this->addItemDetail(item);
}

int ItemListWidget::findRow(const todo::ItemDetail &item) {
    return this->findRowByID(item.getId());
}

int ItemListWidget::findRowByID(const QString &itemID) {
    int targetRow = -1;
    for (int i = 0; i < this->itemModel->rowCount(); ++i) {
        todo::ItemDetail rowItem = this->itemModel->item(i)->data(Qt::UserRole + 1).value<todo::ItemDetail>();
        if (rowItem.getId() == itemID) {
            targetRow = i;
            break;
        }
    }
    return targetRow;
}

void ItemListWidget::setCurrentItemByID(const QString &itemID) {
    int targetRow = this->findRowByID(itemID);
    if (targetRow < 0) {
        return;
    }

    // set cursor to it
//    this->listView->setCurrentIndex()
}