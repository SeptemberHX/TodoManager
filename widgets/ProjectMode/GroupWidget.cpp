#include "GroupWidget.h"
#include "ui_groupwidget.h"
#include "../ItemListItemDelegate.h"
#include <QDebug>

GroupWidget::GroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupWidget)
{
    ui->setupUi(this);

    this->groupDetailWidget = new GroupDetailWidget(this);
    ui->stackedWidget->addWidget(this->groupDetailWidget);
    this->itemDetailWidget = new ItemDetailWidget(this);
    ui->stackedWidget->addWidget(this->itemDetailWidget);
    ui->stackedWidget->setCurrentWidget(this->groupDetailWidget);

    this->listWidget = new ItemListWidget(this);

    this->mainSplitter = new QSplitter(this);
    this->mainSplitter->addWidget(this->listWidget);
    this->mainSplitter->addWidget(ui->stackedWidget);
    ui->horizontalLayout->addWidget(this->mainSplitter);

    connect(this->listWidget, &ItemListWidget::selectedItemChanged, this, &GroupWidget::selected_item_changed);

    // For test
    todo::ItemGroup testGroup;
    testGroup.setTitle("这是一个测试，一个非常非常长的一个测试。");
    testGroup.setMileStone(true);
    testGroup.setDescription("这是一个很长的测试字符串。你说什么，我听不见。\n散人干不死\n哈哈哈");
    QList<todo::ItemAndGroupWrapper> wrappers{todo::ItemAndGroupWrapper(testGroup)};

    todo::ItemDetail testDetail;
    testDetail.setTitle("测试task");
    testDetail.setMode(todo::ItemMode::SIMPLE);
    testDetail.setPriority(2);
    testDetail.setDescription("赶紧睡觉！");
    wrappers.append(todo::ItemAndGroupWrapper(testDetail));
    this->loadItems(wrappers);

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
