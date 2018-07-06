#ifndef TAGMODEWIDGET_H
#define TAGMODEWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QStandardItemModel>
#include "../../data/DataCenter.h"
#include "TagModeListItemDelegate.h"
#include "../../core/ItemTag.h"
#include "../todolistwidget.h"

namespace Ui {
class TagModeWidget;
}

class TagModeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TagModeWidget(QWidget *parent = 0);
    ~TagModeWidget();

    void setItemTags(const QList<todo::ItemTag> &itemTags);

signals:
    void databaseModified();

private slots:
    void list_selected_item_changed();
    void database_modified();

private:
    Ui::TagModeWidget *ui;
    todo::DataCenter dataCenter;

    QStandardItemModel *itemModel;
    TodoListWidget *todoListWidget;
    QSplitter *mainSplitter;
};

#endif // TAGMODEWIDGET_H
