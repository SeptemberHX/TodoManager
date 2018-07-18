#include "GroupDetailWidget.h"
#include "ui_GroupDetailWidget.h"
#include <QString>

GroupDetailWidget::GroupDetailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupDetailWidget)
{
    ui->setupUi(this);
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
}
