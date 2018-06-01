#include "itemaddtagwidget.h"
#include "ui_itemaddtagwidget.h"
#include <QDebug>
#include <QMessageBox>
#include <QToolButton>
#include <QColorDialog>
#include "../data/DataCenter.h"

ItemAddTagWidget::ItemAddTagWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemAddTagWidget),
    acceptFlag(false),
    resultTag(""),
    tagColor(Qt::blue)
{
    ui->setupUi(this);
    QStringList currTagNameList;

    this->currTagLists = this->dataCenter.selectAllItemTag();
    for (auto const &tag : this->currTagLists) {
        currTagNameList << tag.getName();
        this->currTagMap.insert(tag.getId(), tag);
    }
    this->tagCompleter = new QCompleter(currTagNameList, this);
    ui->tagLineEdit->setCompleter(this->tagCompleter);

    connect(ui->buttonBox, QOverload<QAbstractButton*>::of(&QDialogButtonBox::clicked), this, &ItemAddTagWidget::buttonBox_pressed);
    connect(ui->tagLineEdit, &QLineEdit::textChanged, this, &ItemAddTagWidget::tagName_changed);
    connect(ui->colorToolButton, &QToolButton::clicked, this, &ItemAddTagWidget::colorButton_pressed);

    this->setColorButtonColor(this->tagColor);
}

ItemAddTagWidget::~ItemAddTagWidget()
{
    delete ui;
}

void ItemAddTagWidget::buttonBox_pressed(QAbstractButton *btn) {
    if (dynamic_cast<QPushButton*>(btn) == ui->buttonBox->button(QDialogButtonBox::Ok)) {
        if (ui->tagLineEdit->text().isEmpty()) {
            QMessageBox::information(this, tr("Can't Accept !"), tr("Tag name is empty !!!"));
        } else {
            QString tagName = ui->tagLineEdit->text();
            if (this->currTagMap.find(tagName) != this->currTagMap.end()) {
                resultTag = this->currTagMap[tagName];
            } else {
                resultTag = todo::ItemTag(tagName);
                resultTag.setColor(this->tagColor);
                this->dataCenter.insertItemTag(this->resultTag);
            }
            this->acceptFlag = true;
        }
    }
    this->close();
}

bool ItemAddTagWidget::isAcceptFlag() const {
    return acceptFlag;
}

const todo::ItemTag &ItemAddTagWidget::getResultTag() const {
    return resultTag;
}

void ItemAddTagWidget::tagName_changed() {
    if (this->currTagMap.find(ui->tagLineEdit->text()) != this->currTagMap.end()) {
        ui->colorToolButton->setDisabled(true);
        this->setColorButtonColor(this->currTagMap[ui->tagLineEdit->text()].getColor());
    } else {
        ui->colorToolButton->setDisabled(false);
        this->setColorButtonColor(this->tagColor);
    }
}

void ItemAddTagWidget::colorButton_pressed() {
    this->tagColor = QColorDialog::getColor(this->tagColor);
    this->setColorButtonColor(this->tagColor);
}

void ItemAddTagWidget::setColorButtonColor(const QColor &color) {
    QString values = QString("%1, %2, %3, %4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    ui->colorToolButton->setStyleSheet("QToolButton { background-color: rgba("+values+"); }");
}
