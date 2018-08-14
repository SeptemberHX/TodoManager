#include "StickyNoteWidget.h"
#include "ui_StickyNoteWidget.h"
#include <QPainter>
#include <QtMath>
#include <QGraphicsDropShadowEffect>
#include <QString>
#include <QDebug>
#include "../../utils/ItemUtils.h"

StickyNoteWidget::StickyNoteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StickyNoteWidget)
{
    ui->setupUi(this);
    this->itemModel = new QStandardItemModel(ui->listView);
    ui->listView->setModel(this->itemModel);

    this->initWidgetStyle();
    connect(ui->closeToolButton, &QToolButton::clicked, this, &StickyNoteWidget::close);

    connect(this->itemModel, &QStandardItemModel::itemChanged, this, &StickyNoteWidget::list_item_changed);
    connect(&this->dataCenter, &todo::DataCenter::databaseModified, this, &StickyNoteWidget::database_modified);

    this->loadItemsByDate(QDate::currentDate());
}

StickyNoteWidget::~StickyNoteWidget()
{
    delete ui;
}

void StickyNoteWidget::initWidgetStyle() {
    ui->closeToolButton->setIcon(QIcon::fromTheme("window-close"));
    ui->editToolButton->setIcon(QIcon::fromTheme("edit"));
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setStyleSheet(this->getStyleSheet("#FAF9DE", Qt::black));
    ui->listView->setSelectionMode(QAbstractItemView::NoSelection);
}

void StickyNoteWidget::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
}

void StickyNoteWidget::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);
}

void StickyNoteWidget::mouseReleaseEvent(QMouseEvent *event) {
    QWidget::mouseReleaseEvent(event);
}

QString StickyNoteWidget::getStyleSheet(const QColor &bgColor, const QColor &fontColor) {
    QString styleSheetStr(R"(
* {
    background-color: %1;
    color: %2;
}
)");
    return styleSheetStr.arg(bgColor.name()).arg(fontColor.name());
}

void StickyNoteWidget::loadItemsByDate(const QDate &targetDate) {
    this->itemModel->clear();
    auto itemList = this->dataCenter.selectItemDetailByDate(targetDate);
    foreach (auto const &item, itemList) {
        auto listItemPtr = new QStandardItem(item.getTitle());
        listItemPtr->setCheckable(true);
        if (item.isDone()) {
            listItemPtr->setCheckState(Qt::Checked);
        } else {
            listItemPtr->setCheckState(Qt::Unchecked);
        }
        auto f = listItemPtr->font();
        f.setStrikeOut(item.isDone());
        listItemPtr->setFont(f);
        listItemPtr->setToolTip(todo::ItemUtils::generateToolTip(item));
        listItemPtr->setData(QVariant::fromValue(item), Qt::UserRole + 1);
        listItemPtr->setEditable(false);
        this->itemModel->appendRow(listItemPtr);
    }
}

void StickyNoteWidget::list_item_changed(QStandardItem *item) {
    auto itemDetail = item->data(Qt::UserRole + 1).value<todo::ItemDetail>();
    qDebug() << itemDetail.getTitle() << item->checkState();
    this->dataCenter.updateDoneByID(itemDetail.getId(), item->checkState());

    auto f = item->font();
    if (item->checkState()) {
        f.setStrikeOut(true);
    } else {
        f.setStrikeOut(false);
    }
    item->setFont(f);
}

void StickyNoteWidget::database_modified() {
    emit databaseModified();
}
