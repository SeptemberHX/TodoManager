#include "ProjectChooseDialog.h"
#include "ui_ProjectChooseDialog.h"
#include <QDebug>
#include <QPushButton>

ProjectChooseDialog::ProjectChooseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectChooseDialog)
{
    ui->setupUi(this);
    this->itemModel = new QStandardItemModel(0, 1, ui->treeView);
    this->itemModel->setHeaderData(0, Qt::Horizontal, tr("Project name"));
    ui->treeView->setModel(this->itemModel);
    this->loadProjectTree();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    this->setWindowTitle("Select one project...");

    connect(ui->treeView, &QTreeView::clicked, this, &ProjectChooseDialog::treeItem_clicked);
}

ProjectChooseDialog::~ProjectChooseDialog()
{
    delete ui;
    delete this->itemModel;
}

void ProjectChooseDialog::loadProjectTree() {
    auto rootGroups = this->dataCenter.selectItemGroupByType(todo::ItemGroupType::PROJECT);
    foreach (auto const &rootGroup, rootGroups) {
        this->id2GroupMap[rootGroup.getId()] = rootGroup;
        auto nameItemPtr = new QStandardItem(rootGroup.getTitle());
        this->itemModel->appendRow(nameItemPtr);
        this->itemPtr2IDMap.insert(nameItemPtr, rootGroup.getId());
        this->buildProjectTree(nameItemPtr, rootGroup.getSubGroupIDList());
    }
}

void ProjectChooseDialog::buildProjectTree(QStandardItem *parentItem, const QList<QString> &childrenID) {
    auto childrenGroup = this->dataCenter.selectItemGroupByIDs(childrenID);
    QList<QStandardItem*> childItems;
    foreach (auto const &child, childrenGroup) {
        this->id2GroupMap[child.getId()] = child;
        auto nameItemPtr = new QStandardItem(child.getTitle());
        parentItem->appendRow(nameItemPtr);
        this->itemPtr2IDMap.insert(nameItemPtr, child.getId());
        this->buildProjectTree(nameItemPtr, child.getSubGroupIDList());
    }
}

void ProjectChooseDialog::treeItem_clicked(const QModelIndex &index) {
    auto itemPtr = this->itemModel->itemFromIndex(index);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);

    // find the root group id
    auto directGroupItemPtr = itemPtr;
    auto parentItemPtr = itemPtr;
    while (itemPtr != nullptr) {
        parentItemPtr = itemPtr;
        itemPtr = itemPtr->parent();
    }

    this->selectedProjectIDPair = QPair<QString, QString>(this->itemPtr2IDMap[parentItemPtr], this->itemPtr2IDMap[directGroupItemPtr]);
    this->selectedProjectTitlePair = QPair<QString, QString>(
        this->id2GroupMap[this->itemPtr2IDMap[parentItemPtr]].getTitle(),
        this->id2GroupMap[this->itemPtr2IDMap[directGroupItemPtr]].getTitle()
    );
}

const QPair<QString, QString> &ProjectChooseDialog::getSelectedProjectIDPair() const {
    return this->selectedProjectIDPair;
}

const QPair<QString, QString> &ProjectChooseDialog::getSelectedProjectTitlePair() const {
    return this->selectedProjectTitlePair;
}
