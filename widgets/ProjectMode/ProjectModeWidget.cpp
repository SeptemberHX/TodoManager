#include "ProjectModeWidget.h"
#include "ui_ProjectModeWidget.h"

ProjectModeWidget::ProjectModeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectModeWidget)
{
    ui->setupUi(this);
}

ProjectModeWidget::~ProjectModeWidget()
{
    delete ui;
}
