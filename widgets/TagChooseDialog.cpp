#include "TagChooseDialog.h"
#include "ui_TagChooseDialog.h"
#include <QPushButton>
#include <QStandardItem>
#include "TagMode/TagModeListItemDelegate.h"

TagChooseDialog::TagChooseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagChooseDialog)
{
    ui->setupUi(this);
    ui->listView->setItemDelegate(new TagModeListItemDelegate());
    this->itemModel = new QStandardItemModel(ui->listView);
    ui->listView->setModel(this->itemModel);

    auto tagList = this->dataCenter.selectAllItemTag();
    foreach (auto const &tag, tagList) {
        auto itemPtr = new QStandardItem();
        itemPtr->setData(QVariant::fromValue(tag), Qt::UserRole + 1);
        this->itemModel->appendRow(itemPtr);
    }

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->listView, &QListView::clicked, this, &TagChooseDialog::listItem_clicked);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &TagChooseDialog::buttonBox_clicked);
    this->setWindowTitle("Choose one tag...");
}

TagChooseDialog::~TagChooseDialog()
{
    delete ui;
}

void TagChooseDialog::listItem_clicked() {
    if (!ui->buttonBox->button(QDialogButtonBox::Ok)->isEnabled()) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

const QPair<QString, QString> &TagChooseDialog::getSelectedTagNameAndId() const {
    return selectedTagNameAndId;
}

void TagChooseDialog::buttonBox_clicked(QAbstractButton *button) {
    if (dynamic_cast<QPushButton*>(button) == ui->buttonBox->button(QDialogButtonBox::Ok)) {
        auto tag = ui->listView->currentIndex().data(Qt::UserRole + 1).value<todo::ItemTag>();
        this->selectedTagNameAndId = QPair<QString, QString>(tag.getName(), tag.getId());
        this->close();
    }
}
