#include <QCollator>
#include <QLocale>
#include <QDebug>
#include "TagModeWidget.h"
#include "ui_TagModeWidget.h"

bool compareItemTagWithName(const todo::ItemTag &itemTag1, const todo::ItemTag &itemTag2) {
    static QLocale loc(QLocale::Chinese, QLocale::China);
    static QCollator qco(loc);
    return qco.compare(itemTag1.getName(), itemTag2.getName()) < 0;
}

QList<todo::ItemTag> sortItemTagByName(const QList<todo::ItemTag> &itemTagList) {
    QList<todo::ItemTag> itemTagsBeginWithEnglish;
    QList<todo::ItemTag> itemTagsBeginWithChinese;
    for (auto const &itemTag : itemTagList) {
        QChar cha = itemTag.getName().at(1);
        ushort uni = cha.unicode();
        if(uni >= 0x4E00 && uni <= 0x9FA5)  // 这个字符是中文
        {
            itemTagsBeginWithChinese.append(itemTag);
        } else {
            itemTagsBeginWithEnglish.append(itemTag);
        }
    }

    std::sort(itemTagsBeginWithEnglish.begin(), itemTagsBeginWithEnglish.end(), compareItemTagWithName);
    std::sort(itemTagsBeginWithChinese.begin(), itemTagsBeginWithChinese.end(), compareItemTagWithName);
    QList<todo::ItemTag> resultList;
    resultList.append(itemTagsBeginWithEnglish);
    resultList.append(itemTagsBeginWithChinese);
    return resultList;
}

TagModeWidget::TagModeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagModeWidget)
{
    ui->setupUi(this);

    this->todoListWidget = new TodoListWidget(this, TodoListWidgetMode::INBOX);
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

    auto itemTagList = sortItemTagByName(this->dataCenter.selectAllItemTag());
    this->setItemTags(itemTagList);

    connect(ui->listView, &QListView::clicked, this, &TagModeWidget::list_selected_item_changed);
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
    ui->listView->setEnabled(true);
}
