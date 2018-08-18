#include "ProjectModeWidget.h"
#include "ui_ProjectModeWidget.h"
#include "../../utils/ItemGroupUtils.h"
#include "../../utils/StringUtils.h"

ProjectModeWidget::ProjectModeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectModeWidget)
{
    ui->setupUi(this);

    this->navigationBarWidget = new NavigationBarWidget(this);
    this->groupWidget = new GroupWidget(this);

    this->mainVBoxLayout = new QVBoxLayout(this);
    this->mainVBoxLayout->addWidget(this->navigationBarWidget);
    this->mainVBoxLayout->addWidget(this->groupWidget);

    this->setLayout(this->mainVBoxLayout);

    connect(this->groupWidget, &GroupWidget::enterItem, this, &ProjectModeWidget::enter_item);
    connect(this->navigationBarWidget, &NavigationBarWidget::jumpTo, this->groupWidget, QOverload<const QList<QString>&>::of(&GroupWidget::jump_to));
    connect(this->groupWidget, &GroupWidget::databaseModified, this, &ProjectModeWidget::database_modified);
    connect(this->groupWidget, &GroupWidget::jumpToTag, this, &ProjectModeWidget::item_tag_clicked);

    this->setObjectName(todo::StringUtils::generateUniqueID("ProjectModeWidget"));
}

ProjectModeWidget::~ProjectModeWidget()
{
    delete ui;
}

void ProjectModeWidget::enter_item(const QString &itemID, const QString &name) {
    this->navigationBarWidget->append(itemID, name);
}

void ProjectModeWidget::database_modified() {
    emit databaseModified(this->objectName());
}

void ProjectModeWidget::refresh_current_items() {
    this->groupWidget->refresh_current_items();
}

void ProjectModeWidget::jump_to(const QString &itemID) {
    auto fullPathList = todo::ItemGroupUtils::getGroupPath(itemID);
    auto const &itemGroups = this->dataCenter.selectItemGroupByIDs(fullPathList);
    this->navigationBarWidget->clear();
    foreach (auto const &itemGroup, itemGroups) {
        this->navigationBarWidget->append(itemGroup.getId(), itemGroup.getTitle());
    }
    fullPathList.push_front(NavigationBarWidget::ROOT);
    this->groupWidget->jump_to(fullPathList);
}

void ProjectModeWidget::item_tag_clicked(const QString &itemID) {
    emit jumpToTag(itemID);
}
