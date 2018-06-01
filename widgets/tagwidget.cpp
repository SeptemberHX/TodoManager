#include "tagwidget.h"
#include "ui_tagwidget.h"
#include <QDebug>
#include "itemaddtagwidget.h"

TagWidget::TagWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagWidget)
{
    ui->setupUi(this);

    this->showFont = QFont("Aria", 8);
    ui->addTagButton->setFixedSize(30, 30);
    this->tagMargin = QSize(10, 6);
    this->readOnly = false;
    this->tagLayout = new FlowLayout(this);
    this->setLayout(this->tagLayout);

    this->tagLayout->addWidget(ui->addTagButton);

    connect(ui->addTagButton, &QToolButton::clicked, this, &TagWidget::addTagBtn_clicked);
}

TagWidget::~TagWidget()
{
    delete ui;
}

void TagWidget::addTag(const todo::ItemTag &tagItem) {
    if (this->tagMap.find(tagItem.getName()) != this->tagMap.end()) {
        return;
    }

    this->tagMap.insert(tagItem.getName(), tagItem);

    auto *tagButton = new TagLabelWidget(this);
    connect(tagButton, &TagLabelWidget::clicked, this, &TagWidget::tagLabel_clicked);
    tagButton->setText(tagItem.getName());
    tagButton->setTagColor(tagItem.getColor());

//    int tIndex = ui->horizontalLayout->indexOf(ui->addTagButton);
//    ui->horizontalLayout->insertWidget(tIndex, tagButton);

    int tIndex = this->tagLayout->indexOf(ui->addTagButton);
    this->tagLayout->insertWidget(tIndex, tagButton);

    this->tagLabelList.append(tagButton);

    if (this->readOnly) {
        tagButton->setEnabled(false);
    }
}

QList<todo::ItemTag> TagWidget::getTags() {
    return this->tagMap.values();
}

TagLabelWidget *TagWidget::findTagButton(const QString &tagStr) {
    for (auto tagButton : this->tagLabelList) {
        if (tagButton->text() == tagStr) {
            return tagButton;
        }
    }
    return nullptr;
}

void TagWidget::tagLabel_clicked() {
    auto tagButton = dynamic_cast<TagLabelWidget*>(sender());
//    ui->horizontalLayout->removeWidget(tagButton);
    this->tagLayout->removeWidget(tagButton);
    this->tagLabelList.removeOne(tagButton);
    this->tagMap.remove(tagButton->text());
    delete tagButton;

    emit tag_changed();
}

void TagWidget::setReadOnly(bool isReadOnly) {
    this->readOnly = isReadOnly;
    ui->addTagButton->setVisible(!isReadOnly);

    for (auto tagButton : this->tagLabelList) {
        tagButton->setEnabled(!isReadOnly);
    }
}

void TagWidget::resizeAll() {
    for (auto tagButtonPtr : this->tagLabelList) {
        tagButtonPtr->setFont(this->showFont);
        auto textSize = tagButtonPtr->fontMetrics().size(Qt::TextShowMnemonic, tagButtonPtr->text());
        tagButtonPtr->setFixedSize(textSize + this->tagMargin);
    }
}

void TagWidget::setTagFont(const QFont &font) {
    this->showFont = font;
    this->resizeAll();
}

void TagWidget::setTagMargin(int h, int v) {
    this->tagMargin = QSize(h, v);
    this->resizeAll();
}

void TagWidget::clear() {
    this->tagMap.clear();
    for (auto tagButtonPtr : this->tagLabelList) {
        delete tagButtonPtr;
    }
    this->tagLabelList.clear();
}

void TagWidget::addTagBtn_clicked() {
    ItemAddTagWidget addTagWidget(this);
    addTagWidget.setWindowTitle(tr("Add new tag:"));
    addTagWidget.exec();

    if (addTagWidget.isAcceptFlag()) {
        this->addTag(addTagWidget.getResultTag());
        emit(tag_changed());
    }
}
