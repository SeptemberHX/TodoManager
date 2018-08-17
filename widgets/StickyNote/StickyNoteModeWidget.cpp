#include "StickyNoteModeWidget.h"
#include "ui_StickyNoteModeWidget.h"
#include "../../core/EnumTypes.h"

StickyNoteModeWidget::StickyNoteModeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StickyNoteModeWidget)
{
    ui->setupUi(this);

    this->itemDelegate = new StickyNoteTypeItemDelegate(ui->tableView);
    this->itemModel = new QStandardItemModel(ui->tableView);

    this->itemModel->setHorizontalHeaderLabels({"Shown", "Name", "Type", "Created Time", "Last Updated Time"});

    ui->tableView->setModel(this->itemModel);
    ui->tableView->setItemDelegateForColumn(2, this->itemDelegate);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
}

StickyNoteModeWidget::~StickyNoteModeWidget()
{
    delete ui;
}

void StickyNoteModeWidget::appendStickyNote(const todo::StickyNote &stickyNote) {
    auto checkableItemPtr = new QStandardItem();
    checkableItemPtr->setCheckable(true);
    auto nameItemPtr = new QStandardItem(stickyNote.getName());
    auto typeItemPtr = new QStandardItem(stickyNote.getType().toString());
    auto createdTimeItemPtr = new QStandardItem(stickyNote.getCreatedTime().toString("yyyy-MM-dd hh:mm:ss"));
    auto lastUpdatedTimeItemPtr = new QStandardItem(stickyNote.getLastUpdatedTime().toString("yyyy-MM-dd hh:mm:ss"));
    this->itemModel->appendRow({checkableItemPtr, nameItemPtr, typeItemPtr, createdTimeItemPtr, lastUpdatedTimeItemPtr});
}
