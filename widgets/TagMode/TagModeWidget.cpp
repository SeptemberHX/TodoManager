#include <QCollator>
#include <QLocale>
#include <QDebug>
#include <QMessageBox>
#include "../../utils/StringUtils.h"
#include "TagModeWidget.h"
#include "ui_TagModeWidget.h"
#include "../Common/CommonAction.h"

bool compareItemTagWithName(const todo::ItemTag &itemTag1, const todo::ItemTag &itemTag2) {
    return todo::StringUtils::compareString(itemTag1.getName(), itemTag2.getName());
}

TagModeWidget::TagModeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagModeWidget)
{
    ui->setupUi(this);
    this->listView = new CustomListView(this);
    this->listView->setMinimumWidth(200);
    ui->verticalLayout->addWidget(this->listView);

    this->tagListMenu = new QMenu(this);
    this->initRightClickMenu();
    this->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->listView, &QListView::customContextMenuRequested, this, &TagModeWidget::show_tagList_context_menu);

    this->todoListWidget = new TodoListWidget(this, TodoListWidgetMode::TAG);
    this->mainSplitter = new QSplitter(this);
    this->mainSplitter->addWidget(ui->groupBox);
    this->mainSplitter->addWidget(this->todoListWidget);
    this->mainSplitter->setStretchFactor(0, 1);
    this->mainSplitter->setStretchFactor(1, 3);
    ui->horizontalLayout->addWidget(this->mainSplitter);

    this->itemModel = new QStandardItemModel(this->listView);
    this->listView->setItemDelegate(new TagModeListItemDelegate(this->listView));
    this->listView->setModel(this->itemModel);
    this->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->loadTagList();

    connect(this->listView, &CustomListView::currentIndexChanged, this, &TagModeWidget::list_selected_item_changed);
    connect(this->todoListWidget, &TodoListWidget::databaseModified, this, &TagModeWidget::database_modified);
    connect(&this->dataCenter, &todo::DataCenter::databaseModified, this, &TagModeWidget::database_modified);
    connect(this->todoListWidget, &TodoListWidget::jumpToGroup, this, &TagModeWidget::jump_to_group);
    connect(this->todoListWidget, &TodoListWidget::timeRecordOperated, this, &TagModeWidget::timeRecordOperated);

    this->setObjectName(todo::StringUtils::generateUniqueID("TagModeWidget"));
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
    this->listView->setEnabled(false);
    qDebug() << "Selected item changed";
    todo::ItemTag currSelectedTag = this->listView->currentIndex().data(Qt::UserRole + 1).value<todo::ItemTag>();
    auto itemDetailList = this->dataCenter.selectItemDetailsByTag(currSelectedTag);
    qDebug() << "Current selected item is " << currSelectedTag.getName()
             << ", and has " << itemDetailList.count() << " items";
    this->todoListWidget->loadItems(itemDetailList);
    this->listView->setEnabled(true);
}

void TagModeWidget::database_modified() {
    emit databaseModified(this->objectName());
}

void TagModeWidget::refresh_current_items() {
    auto currentRow = this->listView->currentIndex().row();
    this->loadTagList();
    if (this->itemModel->rowCount() > currentRow) {
        this->listView->setCurrentIndex(this->itemModel->index(currentRow, 0));
    }
    this->list_selected_item_changed();
}

void TagModeWidget::clear() {
    this->itemModel->clear();
    // todo: TodoListWidget::clear
}

void TagModeWidget::loadTagList() {
    auto itemTagList = this->dataCenter.selectAllItemTag();
    std::sort(itemTagList.begin(), itemTagList.end(), compareItemTagWithName);
    this->setItemTags(itemTagList);
}

void TagModeWidget::jump_to_group(const QString &groupID) {
    emit jumpToGroup(groupID);
}

void TagModeWidget::initRightClickMenu() {
    QList<CommonActionType> actionTypeList = CommonAction::getInstance()->getTagActions(false);
    foreach (auto const actionType, actionTypeList) {
        QAction *actionPtr = new QAction(CommonAction::getInstance()->getActionName(actionType), this->tagListMenu);
        actionPtr->setObjectName(actionPtr->text());
        connect(actionPtr, &QAction::triggered, this, &TagModeWidget::rightClickMenu_clicked);
        this->tagListMenu->addAction(actionPtr);
    }
}

void TagModeWidget::rightClickMenu_clicked() {
    auto actionType = CommonAction::getInstance()->getActionType(sender()->objectName());
    auto tag = this->listView->selectionModel()->selectedIndexes()[0].data(Qt::UserRole + 1).value<todo::ItemTag>();
    switch (actionType) {
        case CommonActionType::TAG_REMOVE_ONLY:
            if (this->todoListWidget->isEditing()) {
                QMessageBox::information(this, "Attention!", "Save your work first !!");
                return;
            }
            this->deleteTag(tag, false);
            break;
        case CommonActionType::TAG_REMOVE_COMPLETE:
            if (this->todoListWidget->isEditing()) {
                QMessageBox::information(this, "Attention!", "Save your work first !!");
                return;
            }
            this->deleteTag(tag, true);
            break;
        default:
            break;
    }
}

void TagModeWidget::show_tagList_context_menu(const QPoint &point) {
    if (this->itemModel->itemFromIndex(this->listView->indexAt(point)) == nullptr) return;

    this->tagListMenu->popup(QCursor::pos());
}

void TagModeWidget::removeTagFromList(const QString &tagID) {
    for (int i = 0; i < this->itemModel->rowCount(); ++i) {
        auto tag = this->itemModel->item(i)->data(Qt::UserRole + 1).value<todo::ItemTag>();
        if (tagID == tag.getId()) {
            this->itemModel->removeRow(i);
            break;
        }
    }
}

void TagModeWidget::deleteTag(const todo::ItemTag &tag, bool ifDeleteTasks) {
    QString informationStr("Are you sure to delete tag");
    if (ifDeleteTasks) {
        informationStr += " and all tasks belong to it ?";
    } else {
        informationStr += " only ? Tasks will be kept.";
    }

    if (QMessageBox::information(this, "Attention", informationStr, QMessageBox::Ok | QMessageBox::Cancel)
                            == QMessageBox::Cancel) {
        return;
    }

    if (ifDeleteTasks) {
        this->dataCenter.deleteItemTagAndAllItemsByTagId(tag.getId());
    } else {
        this->dataCenter.deleteItemTagOnlyById(tag.getId());
    }
    this->removeTagFromList(tag.getId());
}

void TagModeWidget::jump_to_tag(const QString &tagID) {
    for (int r = 0; r < this->itemModel->rowCount(); ++r) {
        auto tag = this->itemModel->item(r)->data(Qt::UserRole + 1).value<todo::ItemTag>();
        if (tagID == tag.getId()) {
            this->listView->setCurrentIndex(this->itemModel->index(r, 0));
            return;
        }
    }
}
