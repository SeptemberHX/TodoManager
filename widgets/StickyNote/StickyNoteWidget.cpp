#include "StickyNoteWidget.h"
#include "ui_StickyNoteWidget.h"
#include <QString>
#include <QThread>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QSettings>
#include "../../utils/ItemUtils.h"
#include "../../utils/StringUtils.h"
#include "../../config/StickyNoteConfig.h"

StickyNoteWidget::StickyNoteWidget(QWidget *parent) :
    QWidget(parent, Qt::Dialog),
    ui(new Ui::StickyNoteWidget),
    isClickedOnTitle(false),
    isChangedByMySelf(false),
    config("")
{
    ui->setupUi(this);
    this->itemModel = new QStandardItemModel(ui->listView);
    ui->listView->setModel(this->itemModel);

    this->initWidgetStyle();
    connect(ui->closeToolButton, &QToolButton::clicked, this, &StickyNoteWidget::hide);

    connect(this->itemModel, &QStandardItemModel::itemChanged, this, &StickyNoteWidget::list_item_changed);
    connect(&this->dataCenter, &todo::DataCenter::databaseModified, this, &StickyNoteWidget::database_modified);
    this->setObjectName(todo::StringUtils::generateUniqueID("StickyNoteWidget"));
}

StickyNoteWidget::StickyNoteWidget(const todo::StickyNote &stickyNote, QWidget *parent) :
    StickyNoteWidget(parent)
{
    this->loadStickyNote(stickyNote);
    this->loadItemsByDate(QDate::currentDate());
    this->setStickyNoteTitle(QDate::currentDate().toString("yyyy-MM-dd"));
}

StickyNoteWidget::~StickyNoteWidget()
{
    delete ui;
}

void StickyNoteWidget::initWidgetStyle() {
    ui->closeToolButton->setIcon(QIcon::fromTheme("window-close"));
    ui->editToolButton->setIcon(QIcon::fromTheme("edit"));
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint | Qt::Dialog);
    ui->listView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->titleLabel->setAlignment(Qt::AlignCenter);
}

void StickyNoteWidget::mousePressEvent(QMouseEvent *event) {
    if (ui->titleLabel->underMouse()) {
        this->isClickedOnTitle = true;
        this->clickedOnTitlePoint = event->globalPos();
        this->widgetPosWhenClickedOnTitle = this->pos();
    }

    QWidget::mousePressEvent(event);
}

void StickyNoteWidget::mouseMoveEvent(QMouseEvent *event) {
    if (this->isClickedOnTitle) {
        this->move(event->globalPos() - this->clickedOnTitlePoint + this->widgetPosWhenClickedOnTitle);
    }
    QWidget::mouseMoveEvent(event);
}

void StickyNoteWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (this->isClickedOnTitle) {
        this->isClickedOnTitle = false;
        emit widgetMoved(this->pos());
    }

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
    this->isChangedByMySelf = true;
    this->itemModel->clear();
    qDebug() << "all items cleared";
    auto itemList = this->dataCenter.selectItemDetailByDate(targetDate);
    foreach (auto const &item, itemList) {
        auto listItemPtr = new QStandardItem(item.getTitle());
        listItemPtr->setCheckable(true);
        if (item.isDone()) {
            listItemPtr->setCheckState(Qt::Checked);
            listItemPtr->setForeground(Qt::gray);
        } else {
            listItemPtr->setCheckState(Qt::Unchecked);
            listItemPtr->setForeground(Qt::black);
        }
        auto f = listItemPtr->font();
        f.setStrikeOut(item.isDone());
        listItemPtr->setFont(f);
        listItemPtr->setToolTip(todo::ItemUtils::generateToolTip(item));
        listItemPtr->setData(QVariant::fromValue(item), Qt::UserRole + 1);
        listItemPtr->setEditable(false);
        this->itemModel->appendRow(listItemPtr);
    }
    this->isChangedByMySelf = false;
}

void StickyNoteWidget::list_item_changed(QStandardItem *item) {
    if (this->isChangedByMySelf) return;
    this->isChangedByMySelf = true;
    auto itemDetail = item->data(Qt::UserRole + 1).value<todo::ItemDetail>();

    auto f = item->font();
    if (item->checkState()) {
        f.setStrikeOut(true);
        item->setForeground(Qt::gray);
    } else {
        f.setStrikeOut(false);
        item->setForeground(Qt::black);
    }
    item->setFont(f);

    // move it to bottom to avoid databaseModified() signal interrupt current function
    this->dataCenter.updateDoneByID(itemDetail.getId(), item->checkState());
    this->isChangedByMySelf = false;
}

void StickyNoteWidget::database_modified() {
    emit databaseModified();
}

void StickyNoteWidget::setStickyNoteTitle(const QString &noteTitle) {
    ui->titleLabel->setText(noteTitle);
}

void StickyNoteWidget::refresh_current_items() {
    this->loadItemsByDate(QDate::currentDate());
    this->setStickyNoteTitle(QDate::currentDate().toString("yyyy-MM-dd"));
}

QString StickyNoteWidget::getStickyNoteId() const {
    return this->stickyNoteId;
}

void StickyNoteWidget::loadStickyNote(const todo::StickyNote &stickyNote) {
    this->stickyNoteId = stickyNote.getId();
    qDebug() << "Trying to load" << stickyNote.getName() << this->objectName();
    this->move(stickyNote.getPos());
    this->setVisible(stickyNote.isShown());
    this->setStyleSheet(this->getStyleSheet(stickyNote.getBackgroundColor(), stickyNote.getFontColor()));

    // load the sticky note's config from config file
    this->loadConfig();
}

void StickyNoteWidget::loadConfig() {
    QString appConfigDirPath = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation)[0];
    QString stickyNoteConfigFilePath = QDir::cleanPath(
        appConfigDirPath + QDir::separator() + todo::StickyNoteConfig::DIR + QDir::separator() + this->stickyNoteId);
    QSettings sqlConfigSettings(stickyNoteConfigFilePath, QSettings::IniFormat);
    if (!sqlConfigSettings.contains("StickyNote/is_specific_date")) {
        todo::StickyNoteConfig defaultConfig(this->stickyNoteId);
        sqlConfigSettings.beginGroup("StickyNote");
        sqlConfigSettings.setValue("is_specific_date", defaultConfig.isSpecificDate());
        sqlConfigSettings.setValue("from_date", defaultConfig.getFromDate());
        sqlConfigSettings.setValue("to_date", defaultConfig.getToDate());
        sqlConfigSettings.setValue("is_specific_tag", defaultConfig.isSpecificTag());
        sqlConfigSettings.setValue("tagId", defaultConfig.getTagId());
        sqlConfigSettings.setValue("is_specific_project", defaultConfig.isSpecificProject());
        sqlConfigSettings.setValue("projectId", defaultConfig.getProjectId());
        sqlConfigSettings.endGroup();
    }

    todo::StickyNoteConfig resultConfig(this->stickyNoteId);
    resultConfig.setDate(
        sqlConfigSettings.value("StickyNote/from_date").toDate(),
        sqlConfigSettings.value("StickyNote/to_date").toDate(),
        sqlConfigSettings.value("StickyNote/is_specific_date").toBool()
    );
    resultConfig.setProjectId(
        sqlConfigSettings.value("StickyNote/projectId").toString(),
        sqlConfigSettings.value("StickyNote/is_specific_project").toBool()
    );
    resultConfig.setTagId(
        sqlConfigSettings.value("StickyNote/tagId").toString(),
        sqlConfigSettings.value("StickyNote/is_specific_tag").toBool()
    );

    this->config = resultConfig;
}

void StickyNoteWidget::loadItemByConfig(const todo::StickyNoteConfig &config) {
    QList<todo::ItemDetail> itemList;
    if (config.isSpecificDate()) {
        itemList = this->dataCenter.selectItemDetailByDate(config.getFromDate(), config.getToDate());
    }

    if (config.isSpecificProject()) {
        if (!config.isSpecificDate()) {
            // todo
        } else {

        }
    }

    if (config.isSpecificTag()) {
        if (!config.isSpecificProject() && !config.isSpecificTag()) {
            // todo
        } else {

        }
    }
}
