#include "StickyNoteModeWidget.h"
#include "ui_StickyNoteModeWidget.h"
#include "../../core/EnumTypes.h"

StickyNoteModeWidget::StickyNoteModeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StickyNoteModeWidget)
{
    ui->setupUi(this);

    this->itemDelegate = new StickyNoteTypeItemDelegate(ui->tableView);
    ui->tableView->setItemDelegate(this->itemDelegate);
    this->itemModel = new QStandardItemModel(ui->tableView);
    ui->tableView->setModel(this->itemModel);

    this->itemModel->appendRow(new QStandardItem(todo::StickyNoteType::DAILY));
}

StickyNoteModeWidget::~StickyNoteModeWidget()
{
    delete ui;
}
