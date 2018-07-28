#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include <QWidget>
#include <QString>
#include <QSplitter>
#include <QInputDialog>
#include <QStandardItemModel>
#include "GroupDetailWidget.h"
#include "../itemlistwidget.h"
#include "../itemdetailwidget.h"
#include "../../data/DataCenter.h"
#include "../../core/ItemAndGroupWrapper.h"

namespace Ui {
class GroupWidget;
}

class GroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GroupWidget(QWidget *parent = 0);
    ~GroupWidget();

public slots:
    void jump_to(const QList<QString> &pathList);

signals:
    void enterItem(const QString &itemID, const QString &name);

private:
    Ui::GroupWidget *ui;
    GroupDetailWidget *groupDetailWidget;
    ItemDetailWidget *itemDetailWidget;
    ItemListWidget *listWidget;
    QSplitter *mainSplitter;
    todo::DataCenter dataCenter;
    QInputDialog *newItemInputDialog;
    QList<QString> currPathList;

    QMap<QString, todo::ItemAndGroupWrapper> itemMap;

    void loadItems(const QList<todo::ItemAndGroupWrapper> &itemList);
    QList<todo::ItemAndGroupWrapper> getSubItemsForGroup(const QString &groupID);
    QList<todo::ItemAndGroupWrapper> getRootGroups();
    void dealWithNewItem(const todo::ItemAndGroupWrapper &newWrapper);
private slots:
    void selected_item_changed(const QString &itemID);
    void current_item_modified(const todo::ItemAndGroupWrapper &wrapper);
    void item_double_clicked(const QString &itemID);
    void new_group_button_clicked();
    void new_detail_button_clicked();
};

#endif // GROUPWIDGET_H
