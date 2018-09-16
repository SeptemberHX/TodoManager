#include "StickyNoteConfigDialog.h"
#include "ui_StickyNoteConfigDialog.h"
#include "../ProjectChooseDialog.h"
#include <QMessageBox>
#include <QPushButton>

StickyNoteConfigDialog::StickyNoteConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StickyNoteConfigDialog)
{
    ui->setupUi(this);

    connect(ui->projectToolButton, &QToolButton::clicked, this, &StickyNoteConfigDialog::projectToolButton_clicked);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &StickyNoteConfigDialog::buttonBox_clicked);
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
        this->projectId = dialog.getSelectedProjectIDPair().first;
        ui->projectLineEdit->setText(dialog.getSelectedProjectTitlePair().first);
    }
}

void StickyNoteConfigDialog::tagToolButton_clicked() {

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
