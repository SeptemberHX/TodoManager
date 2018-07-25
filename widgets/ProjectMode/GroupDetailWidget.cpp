#include "GroupDetailWidget.h"
#include "ui_GroupDetailWidget.h"
#include <QString>

GroupDetailWidget::GroupDetailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupDetailWidget)
{
    ui->setupUi(this);
    this->changeToViewMode();

    connect(ui->editToolButton, &QToolButton::clicked, this, &GroupDetailWidget::changeToEditMode);
}

GroupDetailWidget::~GroupDetailWidget()
{
    delete ui;
}

void GroupDetailWidget::loadItemGroup(const todo::ItemGroup &itemGroup) {
    ui->titleLineEdit->setText(itemGroup.getTitle());
    ui->descriptionTextEdit->setText(itemGroup.getDescription());
    ui->milestoneCheckBox->setChecked(itemGroup.isMileStone());
    ui->fromDateEdit->setDate(itemGroup.getFromDate());
    ui->toDateEdit->setDate(itemGroup.getToDate());

    QString timeLabelStrTemp("CreatedTime:\n%1\nLastUpdatedTime:\n%2");
    ui->timeLabel->setText(timeLabelStrTemp.arg(itemGroup.getCreatedTime().toString("yyyy-MM-dd"))
                                           .arg(itemGroup.getLastUpdatedTime().toString("yyyy-MM-dd"))
    );

    this->rawItemGroup = itemGroup;  // backup
}

todo::ItemGroup GroupDetailWidget::collectData() const {
    todo::ItemGroup newItemGroup;

    // fill the new object
    newItemGroup.setTitle(ui->titleLineEdit->text());                           // 1
    newItemGroup.setDescription(ui->descriptionTextEdit->toPlainText());        // 2
    newItemGroup.setMileStone(ui->milestoneCheckBox->isChecked());              // 3
    newItemGroup.setFromDate(ui->fromDateEdit->date());                         // 4
    newItemGroup.setToDate(ui->toDateEdit->date());                             // 5
    newItemGroup.setCreatedTime(this->rawItemGroup.getCreatedTime());           // 6
    newItemGroup.setLastUpdatedTime(this->rawItemGroup.getLastUpdatedTime());   // 7
    newItemGroup.setId(this->rawItemGroup.getId());                             // 8
    newItemGroup.setType(this->rawItemGroup.getType());                         // 9

    return newItemGroup;
}

void GroupDetailWidget::item_modified() {
    emit itemModified(todo::ItemAndGroupWrapper(this->collectData()));
}

void GroupDetailWidget::changeToViewMode() {
    this->changeReadOnly(true);
}

void GroupDetailWidget::changeToEditMode() {
    this->changeReadOnly(false);
}

void GroupDetailWidget::changeReadOnly(bool readOnly) const {
    ui->buttonBox->setVisible(!readOnly);
    ui->operationWidget->setVisible(readOnly);
    ui->titleLineEdit->setReadOnly(readOnly);
    ui->descriptionTextEdit->setReadOnly(readOnly);
    ui->fromDateEdit->setReadOnly(readOnly);
    ui->toDateEdit->setReadOnly(readOnly);

    // special trick for checkbox
    ui->milestoneCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents, readOnly);
    ui->milestoneCheckBox->setFocusPolicy(readOnly ? Qt::NoFocus : Qt::StrongFocus);
}

void GroupDetailWidget::connectModifiedSignal() {
    connect(ui->titleLineEdit, &QLineEdit::textChanged, this, &GroupDetailWidget::item_modified);
    connect(ui->descriptionTextEdit, &QTextEdit::textChanged, this, &GroupDetailWidget::item_modified);
    connect(ui->milestoneCheckBox, &QCheckBox::stateChanged, this, &GroupDetailWidget::item_modified);
    connect(ui->fromDateEdit, &QDateEdit::dateChanged, this, &GroupDetailWidget::item_modified);
    connect(ui->toDateEdit, &QDateEdit::dateChanged, this, &GroupDetailWidget::item_modified);
}
