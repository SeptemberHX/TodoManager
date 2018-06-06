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
    explicit TodoListWidget(QWidget *parent = 0);
    ~TodoListWidget();

private:
    Ui::TodoListWidget *ui;
    QSplitter *mainSplitter;
    ItemDetailWidget *detailWidget;
    QInputDialog *newItemInputDialog;
    ItemListWidget *listWidget;
    QLabel *infoLabel;

    bool isCurrentItemEdited;
    todo::ItemDetail currentItem;

    QMap<QString, todo::ItemDetail> currItemDetailMap;

    todo::DataCenter dataCenter;

    void loadItemDetailToListView(const QList<todo::ItemDetail> &itemDetails);

    void dealWithNewItemDetail(const todo::ItemDetail &newItemDetail);
    void saveNewItemDetail(const todo::ItemDetail &newItemDetail);
    void addNewItemDetailToListView(const todo::ItemDetail &newItemDetail);

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
};

#endif // TODOLISTWIDGET_H
