#include "GroupWidget.h"
#include "ui_groupwidget.h"
#include "../ItemListItemDelegate.h"

GroupWidget::GroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupWidget)
{
    ui->setupUi(this);

    this->groupDetailWidget = new GroupDetailWidget(this);
    ui->stackedWidget->addWidget(this->groupDetailWidget);
    ui->stackedWidget->setCurrentWidget(this->groupDetailWidget);

    this->mainSplitter = new QSplitter(this);
    this->mainSplitter->addWidget(ui->listView);
    this->mainSplitter->addWidget(ui->stackedWidget);
    ui->horizontalLayout->addWidget(this->mainSplitter);

    this->itemModel = new QStandardItemModel(this);
    ui->listView->setModel(this->itemModel);
    ui->listView->setItemDelegate(new ItemListItemDelegate(this));

    todo::ItemGroup testGroup;
    testGroup.setTitle("这是一个测试，一个非常非常长的一个测试。");
    this->appendItem(todo::ItemAndGroupWrapper(testGroup));
}

GroupWidget::~GroupWidget()
{
    delete ui;
}

void GroupWidget::appendItem(const todo::ItemAndGroupWrapper &wrapper) {
    QStandardItem *item = new QStandardItem();
    item->setData(QVariant::fromValue(wrapper), Qt::UserRole + 1);
    this->itemModel->appendRow(item);
}
