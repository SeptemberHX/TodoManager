#include <QCollator>
#include <QLocale>
#include <QDebug>
#include "../../utils/StringUtils.h"
#include "TagModeWidget.h"
#include "ui_TagModeWidget.h"

bool compareItemTagWithName(const todo::ItemTag &itemTag1, const todo::ItemTag &itemTag2) {
    return todo::StringUtils::compareString(itemTag1.getName(), itemTag2.getName());
}

TagModeWidget::TagModeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagModeWidget)
{
    ui->setupUi(this);

    this->todoListWidget = new TodoListWidget(this, TodoListWidgetMode::TAG);
    this->mainSplitter = new QSplitter(this);
    this->mainSplitter->addWidget(ui->groupBox);
    this->mainSplitter->addWidget(this->todoListWidget);
    this->mainSplitter->setStretchFactor(0, 1);
    this->mainSplitter->setStretchFactor(1, 3);
    ui->horizontalLayout->addWidget(this->mainSplitter);

    this->itemModel = new QStandardItemModel(ui->listView);
    ui->listView->setItemDelegate(new TagModeListItemDelegate(ui->listView));
    ui->listView->setModel(this->itemModel);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto itemTagList = this->dataCenter.selectAllItemTag();
    std::sort(itemTagList.begin(), itemTagList.end(), compareItemTagWithName);
    this->setItemTags(itemTagList);

    connect(ui->listView, &QListView::clicked, this, &TagModeWidget::list_selected_item_changed);
    connect(this->todoListWidget, &TodoListWidget::databaseModified, this, &TagModeWidget::database_modified);
}

TagModeWidget::~TagModeWidget()
{
    delete ui;
}

void TagModeWidget::setItemTags(const QList<todo::ItemTag> &itemTags) {
    this->itemModel->clear();
    for (auto const &itemTag : itemTags) {
        auto newListItem = new QStandardItem();
        newListItem->setData(QVariant::fromValue(itemTag), Qt::UserRole + 1);
        this->itemModel->insertRow(this->itemModel->rowCount(), newListItem);
    }
}

void TagModeWidget::list_selected_item_changed() {
    ui->listView->setEnabled(false);
    qDebug() << "Selected item changed";
    todo::ItemTag currSelectedTag = ui->listView->currentIndex().data(Qt::UserRole + 1).value<todo::ItemTag>();
    auto itemDetailList = this->dataCenter.selectItemDetailsByTag(currSelectedTag);
    qDebug() << "Current selected item is " << currSelectedTag.getName()
             << ", and has " << itemDetailList.count() << " items";
    this->todoListWidget->loadItems(itemDetailList);
    ui->listView->setEnabled(true);
}

void TagModeWidget::database_modified() {
    emit databaseModified();
}
