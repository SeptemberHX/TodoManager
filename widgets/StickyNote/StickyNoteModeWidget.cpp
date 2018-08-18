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

    this->itemModel->setHorizontalHeaderLabels({"Shown", "Name", "Type", "Font Color", "Background Color"});

    ui->tableView->setModel(this->itemModel);
    ui->tableView->setItemDelegateForColumn(2, this->itemDelegate);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    connect(ui->addToolButton, &QToolButton::clicked, this, &StickyNoteModeWidget::addButton_clicked);
    connect(ui->deleteToolButton, &QToolButton::clicked, this, &StickyNoteModeWidget::deleteButton_clicked);
    connect(this->itemModel, &QStandardItemModel::itemChanged, this, &StickyNoteModeWidget::item_changed);

    ui->addToolButton->setIcon(QIcon::fromTheme("list-add"));
    ui->deleteToolButton->setIcon(QIcon::fromTheme("editdelete"));

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->initStickyNoteWidget();
}

StickyNoteModeWidget::~StickyNoteModeWidget()
{
    delete ui;
}

void StickyNoteModeWidget::appendStickyNote(const todo::StickyNote &stickyNote) {
    auto checkableItemPtr = new QStandardItem();
    checkableItemPtr->setCheckable(true);
    if (stickyNote.isShown()) {
        checkableItemPtr->setCheckState(Qt::Checked);
    } else {
        checkableItemPtr->setCheckState(Qt::Unchecked);
    }

    checkableItemPtr->setData(stickyNote.getId());  // keep the sticky note's unique id
    auto nameItemPtr = new QStandardItem(stickyNote.getName());
    auto typeItemPtr = new QStandardItem(stickyNote.getType().toString());
    auto fontColorItemPtr = new QStandardItem(stickyNote.getFontColor().name());
    auto bgColorItemPtr = new QStandardItem(stickyNote.getBackgroundColor().name());

    QList<QStandardItem*> itemList({checkableItemPtr, nameItemPtr, typeItemPtr, fontColorItemPtr, bgColorItemPtr});
    this->itemModel->appendRow(itemList);
    this->id2ItemListMap[stickyNote.getId()] = itemList;
}

void StickyNoteModeWidget::initStickyNoteWidget() {
    auto stickyNoteList = this->dataCenter.selectAllStickyNote();
    foreach (auto const &stickyNote, stickyNoteList) {
        this->dealWithNewStickyNote(stickyNote);
    }
}

void StickyNoteModeWidget::addButton_clicked() {
    todo::StickyNote newStickyNote;
    newStickyNote.setName("Default");
    this->dataCenter.insertStickyNote(newStickyNote);
    this->dealWithNewStickyNote(newStickyNote);
}

void StickyNoteModeWidget::stickyNoteWidget_moved(const QPoint &newPos) {
    auto sticyNoteWidgetPtr = dynamic_cast<StickyNoteWidget*>(sender());
    this->dataCenter.updateStickyNotePositionById(sticyNoteWidgetPtr->getStickyNoteId(), newPos.x(), newPos.y());
}

void StickyNoteModeWidget::dealWithNewStickyNote(const todo::StickyNote &stickyNote) {
    auto widgetPtr = new StickyNoteWidget(stickyNote);
    connect(widgetPtr, &StickyNoteWidget::widgetMoved, this, &StickyNoteModeWidget::stickyNoteWidget_moved);
    this->id2stickyNoteWidget[stickyNote.getId()] = widgetPtr;
    this->appendStickyNote(stickyNote);
}

void StickyNoteModeWidget::item_changed(QStandardItem *item) {
    QString stickyNoteId = this->itemModel->item(item->row(), 0)->data().toString();
    auto newStickyNote = this->collectStickyNoteByRow(item->row());
    this->dataCenter.updateStickyNote(newStickyNote);
    this->id2stickyNoteWidget[stickyNoteId]->loadSticyNote(newStickyNote);
}

todo::StickyNote StickyNoteModeWidget::collectStickyNoteByRow(int row) {
    todo::StickyNote result;
    result.setId(this->itemModel->item(row, 0)->data().toString());
    if (this->itemModel->item(row, 0)->checkState() == Qt::Checked) {
        result.setShown(true);
    } else {
        result.setShown(false);
    }
    result.setName(this->itemModel->item(row, 1)->text());
    result.setType(this->itemModel->item(row, 2)->text());
    result.setFontColor(this->itemModel->item(row, 3)->text());
    result.setBackgroundColor(this->itemModel->item(row, 4)->text());
    result.setPos(this->id2stickyNoteWidget[result.getId()]->pos());

    return result;
}

void StickyNoteModeWidget::deleteButton_clicked() {
    auto currRowIndex = ui->tableView->currentIndex();
    if (!currRowIndex.isValid()) return;

    QString stickyNoteId = this->itemModel->item(currRowIndex.row(), 0)->data().toString();
    this->dataCenter.deleteStickyNoteById(stickyNoteId);
    delete this->id2stickyNoteWidget[stickyNoteId];
    this->id2stickyNoteWidget.remove(stickyNoteId);
    this->itemModel->removeRow(currRowIndex.row());
}
