#ifndef TODOLISTWIDGET_H
#define TODOLISTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSplitter>
#include <QInputDialog>
#include <QHBoxLayout>
#include "itemdetailwidget.h"
#include "itemlistwidget.h"
#include "../data/DataCenter.h"

namespace Ui {
class TodoListWidget;
}

enum TodoListWidgetMode {
    INBOX,
    DAILY,
    TAG
};

class InboxViewFilterCondition {
public:
    InboxViewFilterCondition();

    const QDate &getTargetFromDate() const;

    void setTargetFromDate(const QDate &targetFromDate);

    const QDate &getTargetToDate() const;

    void setTargetToDate(const QDate &targetToDate);

    bool isShowDoneItems() const;

    void setShowDoneItems(bool showDoneItems);

    bool check(const todo::ItemDetail &item);

private:
    QDate targetFromDate;
    QDate targetToDate;
    bool showDoneItems;
};

/**
 * This class is the parent widget of detail widget and list widget
 * It handles all data operations
 * Remember detail widget and list widget should not change their data by themselves
 * Their data content can only be changed by this class, they are just for view
 *
 * All signal names like this: saveBtnClicked
 * All slot names like this: saveBtn_clicked
 */
class TodoListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TodoListWidget(QWidget *parent = 0, TodoListWidgetMode viewMode = TodoListWidgetMode::DAILY);
    ~TodoListWidget();

    void changeToInboxMode();
    void changeToDailyMode();
    void changeToTagMode();

    /**
     * check whether current item is edited or not
     * used when trying to switch to other view mode
     * @return
     */
    bool isCurrentItemEdited() const;

    /**
     * set TodoListWidget's data.
     * It will change the cache data !!
     * @param itemDetails
     */
    void loadItems(const QList<todo::ItemDetail> &itemDetails);

    bool isEditing() const;

public slots:
    /**
     * read data from DataCenter again.
     */
    void refresh_current_items();

    /**
     * jump to given targetItem: jump to target day and load it to detail widget
     * @param targetItem
     */
    void jump_to_specific_item(const todo::ItemDetail targetItem);
    void jump_to_specific_day(const QDate &targetDay);

private:
    Ui::TodoListWidget *ui;
    QSplitter *mainSplitter;
    ItemDetailWidget *detailWidget;
    QInputDialog *newItemInputDialog;
    ItemListWidget *listWidget;
    QLabel *infoLabel;

    todo::ItemDetail currentItem;
    QMap<QString, todo::ItemDetail> currItemDetailMap;
    todo::DataCenter dataCenter;

    TodoListWidgetMode viewMode;
    InboxViewFilterCondition currentInboxCondition;

    void loadItemDetailToListView(const QList<todo::ItemDetail> &itemDetails);
    void loadItemsByInboxCondition(const InboxViewFilterCondition &cond);

    void dealWithNewItemDetail(const todo::ItemDetail &newItemDetail);
    void saveNewItemDetail(const todo::ItemDetail &newItemDetail);
    void addNewItemDetailToListView(const todo::ItemDetail &newItemDetail);

    void loadTargetDateData(const QDate &targetDate);

    /**
     * Update ItemDetail to database.
     * Some other jobs need to do here, like update currItemDetailMap, etc.
     * @param itemDetail
     */
    void updateItemDetail(const todo::ItemDetail &itemDetail);
    void updateStatusBarInfo();

    void database_modified();

signals:
    void changeStatusBarText(const QString &newStr);
    void databaseModified();  // for notification
    void jumpToGroup(const QString &groupID);
    void jumpToTag(const QString &tagID);

private slots:
    void todayBtn_clicked();

    /**
     * called when date filter changed.
     * List view will load target date's ItemDetails into itself.
     */
    void date_filter_changed();
    void preDayBtn_clicked();
    void nextDayBtn_clicked();

    /**
     * called when list view changing selected item.
     * @param currItemDetailID: new selected item's ID
     */
    void listWidget_selectedItem_changed(const QString &currItemDetailID);
    void addBtn_clicked();
    void current_item_edited(const todo::ItemDetail &currentItemDetail);
    void saveBtn_clicked(const todo::ItemDetail &currentItemDetail);
    void markDone_clicked(bool flag);
    void deleteBtn_clicked();

    void inboxFilter_changed(const InboxViewFilterCondition &newCondition);

    void jump_to_specific_group(const QString &groupID);

    void item_tag_clicked(const QString &itemID);
};

#endif // TODOLISTWIDGET_H
