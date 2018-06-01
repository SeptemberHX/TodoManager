#include "itemlistwidget.h"
#include "ui_itemlistwidget.h"
#include "../utils/itemdetailutils.h"
#include "ItemListItemDelegate.h"
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
}

ItemListWidget::~ItemListWidget()
{
    delete ui;
    this->clearFilters();
}

void ItemListWidget::addItemDetail(const todo::ItemDetail &item)
{
    auto newListItem = new QStandardItem();
    newListItem->setData(QVariant::fromValue(item), Qt::UserRole + 1);
    this->itemModel->insertRow(0, newListItem);
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
}

void ItemListWidget::addDateFilter(const QDate &date) {
    this->filters.append(new todo::DateFilter(date));
}
