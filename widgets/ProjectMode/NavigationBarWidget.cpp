#include "NavigationBarWidget.h"
#include "ui_NavigationBarWidget.h"
#include <QLabel>
#include <QList>
#include <QToolButton>
#include <QDebug>

const QString NavigationBarWidget::ROOT = QString("navigation_root");

NavigationBarWidget::NavigationBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigationBarWidget)
{
    ui->setupUi(this);

    this->widgetPtrList.append(ui->rootToolButton);
    this->buttonObjectName2ID.insert(ui->rootToolButton->objectName(), NavigationBarWidget::ROOT);
    connect(ui->rootToolButton, &QToolButton::clicked, this, &NavigationBarWidget::toolButton_clicked);
}

NavigationBarWidget::~NavigationBarWidget()
{
    delete ui;
}

void NavigationBarWidget::append(const QString &id, const QString &name) {
    QLabel *labelWidget = new QLabel(">", this);
    labelWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    labelWidget->setObjectName(QString("navigationBar_label_%1").arg(id));
    this->widgetPtrList.append(labelWidget);
    ui->horizontalLayout->insertWidget(this->widgetPtrList.size() - 1, labelWidget);

    QToolButton *toolButton = new QToolButton(this);
    toolButton->setText(name);
    toolButton->setObjectName(QString("navigationBar_toolButton_%1").arg(id));
    this->widgetPtrList.append(toolButton);
    this->buttonObjectName2ID.insert(toolButton->objectName(), id);
    connect(toolButton, &QToolButton::clicked, this, &NavigationBarWidget::toolButton_clicked);
    ui->horizontalLayout->insertWidget(this->widgetPtrList.size() - 1, toolButton);
}

void NavigationBarWidget::toolButton_clicked() {
    QString senderObjectName = QObject::sender()->objectName();
    if (this->buttonObjectName2ID.find(senderObjectName) != this->buttonObjectName2ID.end()) {
        QList<QWidget*> newWidgetPtrList;
        bool meetTheWidget = false;
        foreach (auto wPtr, this->widgetPtrList) {
            if (!meetTheWidget) {
                newWidgetPtrList.append(wPtr);
            } else {
                meetTheWidget = true;
                this->buttonObjectName2ID.remove(wPtr->objectName());
                delete wPtr;
                continue;
            }

            if (wPtr->objectName() == senderObjectName) {
                meetTheWidget = true;
            }
        }
        this->widgetPtrList = newWidgetPtrList;
        emit jumpTo(this->buttonObjectName2ID[senderObjectName]);
    }
}
