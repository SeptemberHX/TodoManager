#include "TagModeWidget.h"
#include "ui_TagModeWidget.h"

TagModeWidget::TagModeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagModeWidget)
{
    ui->setupUi(this);

    this->todoListWidget = new TodoListWidget(this, TodoListWidgetMode::INBOX);
    this->mainSplitter = new QSplitter(this);
    this->mainSplitter->addWidget(ui->listView);
    this->mainSplitter->addWidget(this->todoListWidget);
    this->mainSplitter->setStretchFactor(0, 1);
    this->mainSplitter->setStretchFactor(1, 3);
    ui->horizontalLayout->addWidget(this->mainSplitter);

    this->itemModel = new QStandardItemModel(ui->listView);
    ui->listView->setItemDelegate(new TagModeListItemDelegate(ui->listView));
    ui->listView->setModel(this->itemModel);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    todo::ItemTag itemTag("Test");
    itemTag.setColor(Qt::red);
    auto newListItem = new QStandardItem();
    newListItem->setData(QVariant::fromValue(itemTag), Qt::UserRole + 1);
    this->itemModel->insertRow(0, newListItem);

    auto newListItem2 = new QStandardItem();
    newListItem2->setData(QVariant::fromValue(todo::ItemTag()), Qt::UserRole + 1);
    this->itemModel->insertRow(0, newListItem2);
}

TagModeWidget::~TagModeWidget()
{
    delete ui;
}
