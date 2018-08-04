#include "GroupDetailWidget.h"
#include "ui_GroupDetailWidget.h"
#include <QString>
#include <QPushButton>
#include <QColorDialog>
#include <QPieSeries>
#include "../../utils/ItemGroupUtils.h"

GroupDetailWidget::GroupDetailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupDetailWidget)
{
    ui->setupUi(this);
    this->descriptionTextEdit = new MyTextEdit(this);
    ui->groupBoxHorizontalLayout->addWidget(this->descriptionTextEdit);

    connect(ui->editToolButton, &QToolButton::clicked, this, &GroupDetailWidget::changeToEditMode);
    connect(ui->deleteToolButton, &QToolButton::clicked, this, &GroupDetailWidget::delete_button_clicked);
    connect(ui->colorToolButton, &QToolButton::clicked, this, &GroupDetailWidget::color_button_clicked);

    // save or cancel
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &GroupDetailWidget::buttonBox_clicked);
    this->connectModifiedSignal();

    // set icons
    ui->deleteToolButton->setIcon(QIcon::fromTheme("editdelete"));
    ui->editToolButton->setIcon(QIcon::fromTheme("edit"));
    ui->chartview->setRenderHint(QPainter::Antialiasing);

    this->changeToViewMode();
}

GroupDetailWidget::~GroupDetailWidget()
{
    delete ui;
}

void GroupDetailWidget::loadItemGroup(const todo::ItemGroup &itemGroup) {
    this->rawItemGroup = itemGroup;  // backup
    this->changeToViewMode();  // set to view mode first, or will emit itemModified() signal

    ui->titleLineEdit->setText(itemGroup.getTitle());
    this->descriptionTextEdit->setHtml(itemGroup.getDescription());
    ui->milestoneCheckBox->setChecked(itemGroup.isMileStone());
    ui->fromDateEdit->setDate(itemGroup.getFromDate());
    ui->toDateEdit->setDate(itemGroup.getToDate());

    if (itemGroup.getType() == todo::ItemGroupType::PROJECT) {
        ui->projectInfoWidget->hide();
        ui->colorToolButton->show();
        ui->colorToolButton->setStyleSheet(QString("background:%1").arg(itemGroup.getColor().name()));
    } else {
        ui->projectInfoWidget->show();
        auto rootGroup = this->dataCenter.selectItemGroupByID(this->rawItemGroup.getRootGroupID());
        auto directGroup = this->dataCenter.selectItemGroupByID(this->rawItemGroup.getDirectGroupID());
        ui->rootProjectLabel->setText(QString("<a href = %1>%2</a>").arg(rootGroup[0].getId()).arg(rootGroup[0].getTitle()));
        ui->directProjectLabel->setText(QString("<a href = %1>%2</a>").arg(directGroup[0].getId()).arg(directGroup[0].getTitle()));
        ui->colorToolButton->hide();
    }
    this->currentColor = itemGroup.getColor();
    this->rootGroupID = itemGroup.getRootGroupID();
    this->directGroupID = itemGroup.getDirectGroupID();
    QString timeLabelStrTemp("CreatedTime:\n%1\nLastUpdatedTime:\n%2");
    ui->timeLabel->setText(timeLabelStrTemp.arg(itemGroup.getCreatedTime().toString("yyyy-MM-dd  hh:mm:ss"))
                                           .arg(itemGroup.getLastUpdatedTime().toString("yyyy-MM-dd  hh:mm:ss"))
    );

    auto overview = todo::ItemGroupUtils::getGroupOverview(itemGroup);
    ui->subGroupCountLabel->setText(QString::number(overview.getSubGroupCount()));
    ui->subGroupDoneCountLabel->setText(QString::number(overview.getSubGroupDoneCount()));
    ui->subItemCounLabel->setText(QString::number(overview.getSubItemCount()));
    ui->subItemDoneCountLabel->setText(QString::number(overview.getSubItemDoneCount()));
    ui->totalItemCountLabel->setText(QString::number(overview.getTotalItemCount()));
    ui->totalItemDoneCountLabel->setText(QString::number(overview.getTotalItemDoneCount()));

    auto chartPtr = ui->chartview->chart();
    if (chartPtr == nullptr) {
        chartPtr = new QChart();
    }
    chartPtr->removeAllSeries();
    QPieSeries *pieSeries = new QPieSeries(chartPtr);
    int subItemCount = itemGroup.getSubGroupIDList().size() + itemGroup.getItemDetailIDList().size();
    int n1 = 0, n2 = 0, n3 = 0;
    if (subItemCount != 0) {
        n1 = overview.getSubGroupDoneCount() * 100 / subItemCount;
        n2 = (overview.getSubGroupCount() - overview.getSubGroupDoneCount()) * 100 / subItemCount;
        n3 = (overview.getSubItemCount() - overview.getSubItemDoneCount()) * 100 / subItemCount;
    }
    int n4 = 100 - n1 - n2 - n3;
    pieSeries->append("Project F", n1);
    pieSeries->append("Project NF", n2);
    pieSeries->append("Task F", n4);
    pieSeries->append("Task NF", n3);
    chartPtr->addSeries(pieSeries);
    chartPtr->legend()->setAlignment(Qt::AlignBottom);
    chartPtr->setTitle("Project's child items chart");
    ui->chartview->setChart(chartPtr);
}

todo::ItemGroup GroupDetailWidget::collectData() const {
    todo::ItemGroup newItemGroup;

    // fill the new object
    newItemGroup.setTitle(ui->titleLineEdit->text());                           // 1
    newItemGroup.setDescription(this->descriptionTextEdit->toHtml());           // 2
    newItemGroup.setMileStone(ui->milestoneCheckBox->isChecked());              // 3
    newItemGroup.setFromDate(ui->fromDateEdit->date());                         // 4
    newItemGroup.setToDate(ui->toDateEdit->date());                             // 5
    newItemGroup.setCreatedTime(this->rawItemGroup.getCreatedTime());           // 6
    newItemGroup.setLastUpdatedTime(this->rawItemGroup.getLastUpdatedTime());   // 7
    newItemGroup.setId(this->rawItemGroup.getId());                             // 8
    newItemGroup.setType(this->rawItemGroup.getType());                         // 9
    newItemGroup.setColor(this->currentColor);                                  // 10
    newItemGroup.setDirectGroupID(this->directGroupID);                         // 11
    newItemGroup.setRootGroupID(this->rootGroupID);                             // 12

    return newItemGroup;
}

void GroupDetailWidget::item_modified() {
    if (!this->readOnlyFlag) {
        emit itemModified(todo::ItemAndGroupWrapper(this->collectData()));
    }
}

void GroupDetailWidget::changeToViewMode() {
    this->changeReadOnly(true);
}

void GroupDetailWidget::changeToEditMode() {
    this->changeReadOnly(false);
}

void GroupDetailWidget::changeReadOnly(bool readOnly) {
    ui->buttonBox->setVisible(!readOnly);
    ui->operationWidget->setVisible(readOnly);
    ui->titleLineEdit->setReadOnly(readOnly);
    this->descriptionTextEdit->setReadOnly(readOnly);
    ui->fromDateEdit->setReadOnly(readOnly);
    ui->toDateEdit->setReadOnly(readOnly);
    ui->colorToolButton->setEnabled(!readOnly);

    // special trick for checkbox
    ui->milestoneCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents, readOnly);
    ui->milestoneCheckBox->setFocusPolicy(readOnly ? Qt::NoFocus : Qt::StrongFocus);

    this->readOnlyFlag = readOnly;
}

void GroupDetailWidget::connectModifiedSignal() {
    connect(ui->titleLineEdit, &QLineEdit::textChanged, this, &GroupDetailWidget::item_modified);
    connect(this->descriptionTextEdit, &MyTextEdit::textChanged, this, &GroupDetailWidget::item_modified);
    connect(ui->milestoneCheckBox, &QCheckBox::stateChanged, this, &GroupDetailWidget::item_modified);
    connect(ui->fromDateEdit, &QDateEdit::dateChanged, this, &GroupDetailWidget::item_modified);
    connect(ui->toDateEdit, &QDateEdit::dateChanged, this, &GroupDetailWidget::item_modified);
}

void GroupDetailWidget::delete_button_clicked() {
    emit deleteButtonClicked();
}

bool GroupDetailWidget::isEditing() {
    return !this->readOnlyFlag;
}

void GroupDetailWidget::buttonBox_clicked(QAbstractButton *btn) {
    if (dynamic_cast<QPushButton*>(btn) == ui->buttonBox->button(QDialogButtonBox::Save)) {
        if (ui->titleLineEdit->text().size() > 0) {
            emit saveActionTriggered(this->collectData());
        } else {
            return;
        }
    } else {
        this->loadItemGroup(this->rawItemGroup);
    }

    this->changeToViewMode();
}

void GroupDetailWidget::color_button_clicked() {
    auto newColor = QColorDialog::getColor(this->currentColor);
    if (newColor != this->currentColor) {
        this->currentColor = newColor;
        ui->colorToolButton->setStyleSheet(QString("background:%1").arg(newColor.name()));
        this->item_modified();
    }
}
