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
    this->itemModel = new QStandardItemModel(ui->listView);
    ui->listView->setModel(this->itemModel);
    ui->listView->setItemDelegate(new ItemListItemDelegate(ui->listView));
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setMinimumWidth(430);
//    ui->listView->setDragEnabled(true);
//    ui->listView->setDragDropMode(QAbstractItemView::DragDrop);

    connect(ui->listView, &QListView::clicked, this, &ItemListWidget::listWidget_selectedItem_changed);

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
        this->addItemDetail_(item);
    }
}

void ItemListWidget::refresh_item_info(const todo::ItemDetail &item)
{
    // have no idea which row 'item' at. Search all data
    for (int i = 0; i < this->itemModel->rowCount(); ++i) {
        todo::ItemDetail rowItem = this->itemModel->item(i)->data(Qt::UserRole + 1).value<todo::ItemDetail>();
        if (rowItem.getId() == item.getId()) {
            this->itemModel->item(i)->setData(QVariant::fromValue(item), Qt::UserRole + 1);
            break;
        }
    }
}

void ItemListWidget::listWidget_selectedItem_changed() {
    todo::ItemDetail currItemDetail = ui->listView->currentIndex().data(Qt::UserRole + 1).value<todo::ItemDetail>();
    emit selectedItemChanged(currItemDetail.getId());
}

void ItemListWidget::loadItemDetails(const QList<todo::ItemDetail> &items) {
    this->itemModel->clear();
    QList<todo::ItemDetail> itemsAfterFilter = items;
    for (auto filterPtr : this->filters) {
        itemsAfterFilter = filterPtr->filter(itemsAfterFilter);
    }

    itemsAfterFilter = this->sortItemlist(itemsAfterFilter);
    for (auto item: itemsAfterFilter) {
        this->addItemDetail(item);  // Use addItemDetail method to sort new item according to sorters.
    }
}

void ItemListWidget::removeItemDetailByID(const QString &itemID) {
    todo::ItemDetail prevItemDetail = ui->listView->currentIndex().data(Qt::UserRole + 1).value<todo::ItemDetail>();
    qDebug() << prevItemDetail.getTitle();
    for (int i = 0; i < this->itemModel->rowCount(); ++i) {
        todo::ItemDetail rowItem = this->itemModel->item(i)->data(Qt::UserRole + 1).value<todo::ItemDetail>();
        if (rowItem.getId() == itemID) {
            this->itemModel->removeRow(i);
            break;
        }
    }
    todo::ItemDetail currItemDetail = ui->listView->currentIndex().data(Qt::UserRole + 1).value<todo::ItemDetail>();
    qDebug() << currItemDetail.getTitle();
    if (currItemDetail.getId() != prevItemDetail.getId()) {
        emit selectedItemChanged(currItemDetail.getId());
    }
}

void ItemListWidget::clearFilters() {
    for (auto ptr : this->filters) {
        delete ptr;
    }
    this->filters.clear();
}

QList<todo::ItemDetail> ItemListWidget::filtItemList(const QList<todo::ItemDetail> &details) {
    QList<todo::ItemDetail> resultLists = details;
    for (auto filterPtr : this->filters) {
        resultLists = filterPtr->filter(resultLists);
    }
    return resultLists;
}

bool ItemListWidget::checkItem(const todo::ItemDetail &detail) {
    bool passCheckFlag = true;
    for (auto filterPtr : this->filters) {
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
        this->refresh_item_info(item);
    } else {
        this->removeItemDetailByID(item.getId());
    }
}

void ItemListWidget::clearSorters() {
    for (auto ptr : this->sorters) {
        delete ptr;
    }
    this->sorters.clear();
}

QList<todo::ItemDetail> ItemListWidget::sortItemlist(const QList<todo::ItemDetail> &details) {
    QList<todo::ItemDetail> results(details);
    std::sort(results.begin(), results.end(), todo::SorterOrganize(this->sorters));
    return results;
}
