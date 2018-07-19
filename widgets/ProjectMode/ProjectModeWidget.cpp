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
}

ProjectModeWidget::~ProjectModeWidget()
{
    delete ui;
}
