#include "itemlistitemwidget.h"
#include "ui_itemlistitemwidget.h"
#include <QString>
#include <QDebug>

ItemListItemWidget::ItemListItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemListItemWidget),
    itemDetailPtr(nullptr)
{
    ui->setupUi(this);
    ui->descriptionLabel->setStyleSheet("QLabel { color: LightSlateGray; }");
    ui->priorityLabel->setStyleSheet("QLabel { color: LightSlateGray; }");
    ui->timeLabel->setStyleSheet("QLabel { color: LightSlateGray; }");
}

ItemListItemWidget::~ItemListItemWidget()
{
    delete ui;
}

void ItemListItemWidget::setTagColor(const QColor &color)
{
    QString values = QString("%1, %2, %3, %4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    ui->colorLabel->setStyleSheet("QLabel { background-color: rgba("+values+"); }");
}

void ItemListItemWidget::loadItemDetail(const todo::ItemDetail &item)
{
    this->itemDetailPtr = std::make_shared<todo::ItemDetail>(item);
    this->reloadItemDetail();
}

void ItemListItemWidget::resizeEvent(QResizeEvent *event) {
    this->reloadItemDetail();
    QWidget::resizeEvent(event);

    auto item = this->itemDetailPtr.get();
//    qDebug() << item->getId() << ": " << this->width() << " - " << this->height();
}

void ItemListItemWidget::reloadItemDetail() {
    if (this->itemDetailPtr == nullptr) {
        return;
    }

    auto item = this->itemDetailPtr.get();
    int titleStrWidth = ui->titleLabel->fontMetrics().width(item->getTitle());
    if (titleStrWidth > ui->titleLabel->width()) {
        ui->titleLabel->setText(
                ui->titleLabel->fontMetrics().elidedText(
                        item->getTitle(),
                        Qt::ElideRight,
                        ui->titleLabel->width()
                )
        );
    } else {
        ui->titleLabel->setText(item->getTitle());
    }

    QString descriptionStr = item->getDescription();
    descriptionStr.replace('\n', ' ');
    int descriptionStrWidth = ui->descriptionLabel->fontMetrics().width(descriptionStr, -1, Qt::TextWordWrap);
    if (descriptionStrWidth > ui->descriptionLabel->width()) {
        ui->descriptionLabel->setText(
                ui->descriptionLabel->fontMetrics().elidedText(
                        descriptionStr,
                        Qt::ElideRight,
                        ui->descriptionLabel->width()
                )
        );
    } else {
        ui->descriptionLabel->setText(descriptionStr);
    }

    ui->priorityLabel->setText(QString("Priority: ") + QString::number(item->getPriority()));
    ui->priorityLabel->adjustSize();
    ui->timeLabel->setText(item->getFromTime().toString("hh:mm") + "-" + item->getToTime().toString("hh:mm"));
    ui->timeLabel->adjustSize();

    QList<todo::ItemTag> tags = item->getTags();
    if (tags.size() > 0) {
        this->setTagColor(tags[0].getColor());
    }

    this->setDoneLabelColor(item->isDone());
}

void ItemListItemWidget::setDoneLabelColor(bool isDone)
{
    if (isDone) {
        QColor color("#90EE90");
        QString values = QString("%1, %2, %3, %4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        ui->doneLabel->setStyleSheet("QLabel { background-color: rgba("+values+"); }");
    }
}
