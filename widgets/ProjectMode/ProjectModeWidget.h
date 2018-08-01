#ifndef PROJECTMODEWIDGET_H
#define PROJECTMODEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "GroupWidget.h"
#include "NavigationBarWidget.h"
#include "../../data/DataCenter.h"

namespace Ui {
class ProjectModeWidget;
}

class ProjectModeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectModeWidget(QWidget *parent = 0);
    ~ProjectModeWidget();
    void refresh_current_items();

public slots:
    void jump_to(const QString &itemID);

signals:
    void databaseModified();

private:
    todo::DataCenter dataCenter;
    Ui::ProjectModeWidget *ui;
    QVBoxLayout *mainVBoxLayout;
    GroupWidget *groupWidget;
    NavigationBarWidget *navigationBarWidget;

private slots:
    void enter_item(const QString &itemID, const QString &name);
    void database_modified();
};

#endif // PROJECTMODEWIDGET_H
