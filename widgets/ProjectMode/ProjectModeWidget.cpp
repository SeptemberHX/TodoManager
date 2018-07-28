#include "ProjectModeWidget.h"
#include "ui_ProjectModeWidget.h"

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
    connect(this->navigationBarWidget, &NavigationBarWidget::jumpTo, this->groupWidget, &GroupWidget::jump_to);
    connect(this->groupWidget, &GroupWidget::databaseModified, this, &ProjectModeWidget::database_modified);
}

ProjectModeWidget::~ProjectModeWidget()
{
    delete ui;
}

void ProjectModeWidget::enter_item(const QString &itemID, const QString &name) {
    this->navigationBarWidget->append(itemID, name);
}

void ProjectModeWidget::database_modified() {
    emit databaseModified();
}

void ProjectModeWidget::refresh_current_items() {
    this->groupWidget->refresh_current_items();
}
