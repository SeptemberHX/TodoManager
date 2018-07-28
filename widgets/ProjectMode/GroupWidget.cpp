#include "GroupWidget.h"
#include "ui_groupwidget.h"
#include "../ItemListItemDelegate.h"
#include "../../core/SqlErrorException.h"
#include "NavigationBarWidget.h"
#include <QDebug>
#include <QMessageBox>

GroupWidget::GroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupWidget)
{
    ui->setupUi(this);

    this->newItemInputDialog = new QInputDialog(this);

    this->groupDetailWidget = new GroupDetailWidget(this);
    ui->stackedWidget->addWidget(this->groupDetailWidget);
    this->itemDetailWidget = new ItemDetailWidget(this);
    ui->stackedWidget->addWidget(this->itemDetailWidget);
    ui->stackedWidget->setCurrentWidget(this->groupDetailWidget);

    this->listWidget = new ItemListWidget(ui->leftWidget);
    ui->leftVLayout->addWidget(this->listWidget);

    this->mainSplitter = new QSplitter(this);
    this->mainSplitter->addWidget(ui->leftWidget);
    this->mainSplitter->addWidget(ui->stackedWidget);
    this->mainSplitter->setStretchFactor(0, 1);
    this->mainSplitter->setStretchFactor(1, 3);
    ui->horizontalLayout->addWidget(this->mainSplitter);

    connect(this->listWidget, &ItemListWidget::selectedItemChanged, this, &GroupWidget::selected_item_changed);
    connect(this->groupDetailWidget, &GroupDetailWidget::itemModified, this, &GroupWidget::current_item_modified);
    connect(this->listWidget, &ItemListWidget::doubleClicked, this, &GroupWidget::item_double_clicked);

    connect(ui->addItemGroupToolButton, &QToolButton::clicked, this, &GroupWidget::new_group_button_clicked);
    connect(ui->addItemDetailToolButton, &QToolButton::clicked, this, &GroupWidget::new_detail_button_clicked);

    this->loadItems(this->getRootGroups());
    this->currPathList.append(NavigationBarWidget::ROOT);
}

GroupWidget::~GroupWidget()
{
    delete ui;
}

void GroupWidget::selected_item_changed(const QString &itemID) {
    if (this->itemMap[itemID].isGroup()) {
        ui->stackedWidget->setCurrentWidget(this->groupDetailWidget);
        this->groupDetailWidget->loadItemGroup(this->itemMap[itemID].getItemGroup());
    } else {
        ui->stackedWidget->setCurrentWidget(this->itemDetailWidget);
        this->itemDetailWidget->loadItemDetail(this->itemMap[itemID].getItemDetail());
    }
}

void GroupWidget::loadItems(const QList<todo::ItemAndGroupWrapper> &itemList) {
    this->itemMap.clear();
    foreach (auto const &item, itemList) {
        this->itemMap.insert(item.getID(), item);
    }

    this->listWidget->loadItemWrappers(itemList);
}

void GroupWidget::current_item_modified(const todo::ItemAndGroupWrapper &wrapper) {
    this->listWidget->refresh_item_info(wrapper);
}

void GroupWidget::item_double_clicked(const QString &itemID) {
    qDebug() << itemID << "double clicked";
    if (this->itemMap[itemID].isGroup()) {
        auto subItemList = this->getSubItemsForGroup(itemID);
        emit enterItem(itemID, this->itemMap[itemID].getItemGroup().getTitle());
        this->loadItems(subItemList);
        this->currPathList.append(itemID);
    }
}

QList<todo::ItemAndGroupWrapper> GroupWidget::getSubItemsForGroup(const QString &groupID) {
    return this->dataCenter.selectItemByDirectGroupID(groupID);
}

QList<todo::ItemAndGroupWrapper> GroupWidget::getRootGroups() {
     auto groupList = this->dataCenter.selectItemGroupByType(todo::ItemGroupType::PROJECT);
     QList<todo::ItemAndGroupWrapper> wrapperList;
     foreach (auto const &group, groupList) {
         wrapperList.append(group);
     }
     return wrapperList;
}

void GroupWidget::jump_to(const QList<QString> &pathList) {
    QString clickedID = pathList.last();
    if (clickedID != NavigationBarWidget::ROOT) {
        this->loadItems(this->getSubItemsForGroup(clickedID));
    } else {
        this->loadItems(this->getRootGroups());

    }
    this->currPathList = pathList;  // restore the full path
}

void GroupWidget::dealWithNewItem(const todo::ItemAndGroupWrapper &newWrapper) {
    // 1. save it to database
    try {
        if (newWrapper.isGroup()) {
            this->dataCenter.insertItemGroup(newWrapper.getItemGroup());
        } else {
            this->dataCenter.insertItemDetail(newWrapper.getItemDetail());
        }
    } catch (const todo::SqlErrorException &e) {
        qDebug() << "Save item in GroupWidget failed";
        return;
    }

    // 2. save the relation
    todo::ItemGroupRelation relation;
    relation.setItemID(newWrapper.getID());
    relation.setDirectGroupID(this->currPathList.last());
    relation.setRootGroupID(this->currPathList[1]);
    this->dataCenter.insertItemGroupRelation(relation);

    // 3. save it to current map
    this->itemMap.insert(newWrapper.getID(), newWrapper);

    // 4. load it to list
    this->listWidget->addItemWrapper(newWrapper);
}

void GroupWidget::new_group_button_clicked() {
    this->newItemInputDialog->setTextValue(tr(""));
    int ok = this->newItemInputDialog->exec();
    if (ok) {
        QString title = this->newItemInputDialog->textValue();
        if (title.size() == 0) {
            QMessageBox::information(this, tr("Invalid input !"), tr("Can't leave it empty !!!"));
            return;
        } else {
            todo::ItemGroup itemGroup;
            itemGroup.setTitle(title);
            if (this->currPathList.last() != NavigationBarWidget::ROOT) {
                itemGroup.setType(todo::ItemGroupType::SUB_PROJECT);
            } else {
                itemGroup.setType(todo::ItemGroupType::PROJECT);
            }
            this->dealWithNewItem(itemGroup);
        }
    }
}

void GroupWidget::new_detail_button_clicked() {
    this->newItemInputDialog->setTextValue(tr(""));
    int ok = this->newItemInputDialog->exec();
    if (ok) {
        QString title = this->newItemInputDialog->textValue();
        if (title.size() == 0) {
            QMessageBox::information(this, tr("Invalid input !"), tr("Can't leave it empty !!!"));
            return;
        } else {
            todo::ItemDetail newItemDetail(title);
            this->dealWithNewItem(newItemDetail);
        }
    }
}
