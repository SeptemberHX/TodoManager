#include "FetchConfigFilePathWidget.h"
#include "ui_FetchConfigFilePathWidget.h"

#include <QFileDialog>
#include <QMessageBox>

FetchConfigFilePathWidget::FetchConfigFilePathWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FetchConfigFilePathWidget)
{
    ui->setupUi(this);
}

FetchConfigFilePathWidget::~FetchConfigFilePathWidget()
{
    delete ui;
}

const QString &FetchConfigFilePathWidget::getFilePath()
{
    return this->filePath;
}

void FetchConfigFilePathWidget::on_newFileCheckBox_stateChanged(int arg1)
{
    if (arg1) {
        ui->chooseFileCheckBox->setChecked(false);
    }
}

void FetchConfigFilePathWidget::on_chooseFileCheckBox_stateChanged(int arg1)
{
    if (arg1) {
        ui->newFileCheckBox->setChecked(false);
    }
}

void FetchConfigFilePathWidget::on_newFileToolButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileUrl(this).path();
    ui->newFileLineEdit->setText(filePath);
}

void FetchConfigFilePathWidget::on_chooseFileToolButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileUrl(this).path();
    ui->chooseFileLineEdit->setText(filePath);
}

void FetchConfigFilePathWidget::on_buttonBox_clicked(QAbstractButton *button)
{
    if (dynamic_cast<QPushButton*>(button) == ui->buttonBox->button(QDialogButtonBox::Ok)) {
        if (ui->newFileCheckBox->isChecked() && ui->newFileLineEdit->text().size() > 0) {
            this->filePath = ui->newFileLineEdit->text();
            this->close();
        } else if (ui->chooseFileCheckBox->isChecked() && ui->chooseFileLineEdit->text().size() > 0) {
            this->filePath = ui->chooseFileLineEdit->text();
            this->close();
        } else {
            QMessageBox::information(this, tr("Choose db path"), tr("Invaild input"));
        }
    }
}
