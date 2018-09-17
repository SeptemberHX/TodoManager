#include "StickyNoteConfigDialog.h"
#include "ui_StickyNoteConfigDialog.h"

#include <QMessageBox>
#include <QPushButton>
#include "../ProjectChooseDialog.h"
#include "../TagChooseDialog.h"

StickyNoteConfigDialog::StickyNoteConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StickyNoteConfigDialog)
{
    ui->setupUi(this);

    ui->fromDateEdit->setDate(QDate::currentDate());
    ui->toDateEdit->setDate(QDate::currentDate());

    connect(ui->projectToolButton, &QToolButton::clicked, this, &StickyNoteConfigDialog::projectToolButton_clicked);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &StickyNoteConfigDialog::buttonBox_clicked);
    connect(ui->tagToolButton, &QToolButton::clicked, this, &StickyNoteConfigDialog::tagToolButton_clicked);
    connect(ui->fromDateEdit, &QDateEdit::dateChanged, this, &StickyNoteConfigDialog::fromDate_changed);
    connect(ui->toDateEdit, &QDateEdit::dateChanged, this, &StickyNoteConfigDialog::toDate_changed);

    this->setWindowTitle("Sticky Note Config Dialog");
}

StickyNoteConfigDialog::~StickyNoteConfigDialog()
{
    delete ui;
}

todo::StickyNoteConfig StickyNoteConfigDialog::collectConfig() {
    todo::StickyNoteConfig resultConfig;
    resultConfig.setDate(ui->fromDateEdit->date(), ui->toDateEdit->date(), ui->dateGroupBox->isChecked());
    resultConfig.setProjectId(this->projectId, ui->projectGroupBox->isChecked());
    resultConfig.setTagId(this->tagId, ui->tagGroupBox->isChecked());
    resultConfig.setShowFinishedTask(ui->showFinishedTaskCheckBox->isChecked());
    return resultConfig;
}

bool StickyNoteConfigDialog::isInputValid() {
    if (!ui->dateGroupBox->isChecked() && !ui->projectGroupBox->isChecked() && !ui->tagGroupBox->isChecked()) {
        return false;
    }

    if (ui->projectGroupBox->isChecked() && ui->projectLineEdit->text().isEmpty()) {
        return false;
    }

    if (ui->tagGroupBox->isChecked() && ui->tagLineEdit->text().isEmpty()) {
        return false;
    }

    return true;
}

void StickyNoteConfigDialog::projectToolButton_clicked() {
    ProjectChooseDialog dialog;
    if (dialog.exec()) {
        this->projectId = dialog.getSelectedProjectIDPair().second;
        ui->projectLineEdit->setText(dialog.getSelectedProjectTitlePair().second);
    }
}

void StickyNoteConfigDialog::tagToolButton_clicked() {
    TagChooseDialog dialog;
    if (dialog.exec()) {
        this->tagId = dialog.getSelectedTagNameAndId().second;
        ui->tagLineEdit->setText(dialog.getSelectedTagNameAndId().first);
    }
}

void StickyNoteConfigDialog::buttonBox_clicked(QAbstractButton *button) {
    if (dynamic_cast<QPushButton*>(button) == ui->buttonBox->button(QDialogButtonBox::Ok)) {
        if (!this->isInputValid()) {
            QMessageBox::information(this, tr("Sticky note config dialog"), tr("Invaild input"));
            return;
        } else {
            this->close();
        }
    }
}

void StickyNoteConfigDialog::loadConfig(const todo::StickyNoteConfig &config) {
    ui->dateGroupBox->setChecked(config.isSpecificDate());
    ui->fromDateEdit->setDate(config.getFromDate());
    ui->toDateEdit->setDate(config.getToDate());

    ui->projectGroupBox->setChecked(config.isSpecificProject());
    this->projectId = config.getProjectId();
    auto projectList = this->dataCenter.selectItemGroupByID(config.getProjectId());
    if (!projectList.isEmpty()) {
        ui->projectLineEdit->setText(projectList[0].getTitle());
    } else {
        ui->projectLineEdit->setText(config.getProjectId());
    }

    ui->tagGroupBox->setChecked(config.isSpecificTag());
    this->tagId = config.getTagId();
    auto tagList = this->dataCenter.selectItemTagById(config.getTagId());
    if (!tagList.isEmpty()) {
        ui->tagLineEdit->setText(tagList[0].getName());
    } else {
        ui->tagLineEdit->setText(config.getTagId());
    }

    ui->showFinishedTaskCheckBox->setChecked(config.isShowFinishedTask());
}

void StickyNoteConfigDialog::fromDate_changed() {
    if (ui->fromDateEdit->date() > ui->toDateEdit->date()) {
        ui->toDateEdit->setDate(ui->fromDateEdit->date());
    }
}

void StickyNoteConfigDialog::toDate_changed() {
    if (ui->toDateEdit->date() < ui->fromDateEdit->date()) {
        ui->fromDateEdit->setDate(ui->toDateEdit->date());
    }
}
