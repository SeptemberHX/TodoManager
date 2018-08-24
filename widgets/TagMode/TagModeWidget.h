#ifndef TAGMODEWIDGET_H
#define TAGMODEWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QSplitter>
#include <QStandardItemModel>
#include "../../data/DataCenter.h"
#include "TagModeListItemDelegate.h"
#include "../../core/ItemTag.h"
#include "../todolistwidget.h"
#include "../CustomListView.h"

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
    void refresh_current_items();

signals:
    void databaseModified(const QString &senderObjectName);
    void jumpToGroup(const QString &groupID);
    void timeRecordOperated(const QString &itemID, const todo::TaskArchivingOperation operation);

public slots:
    void jump_to_tag(const QString &tagID);

private slots:
    void list_selected_item_changed();
    void database_modified();
    void jump_to_group(const QString &groupID);
    void rightClickMenu_clicked();
    void show_tagList_context_menu(const QPoint &point);

private:
    Ui::TagModeWidget *ui;
    todo::DataCenter dataCenter;

    QMenu *tagListMenu;
    QStandardItemModel *itemModel;
    TodoListWidget *todoListWidget;
    QSplitter *mainSplitter;
    CustomListView *listView;

    void clear();
    void loadTagList();
    void initRightClickMenu();
    void removeTagFromList(const QString &tagID);
    void deleteTag(const todo::ItemTag &tag, bool ifDeleteTasks);
};

#endif // TAGMODEWIDGET_H
