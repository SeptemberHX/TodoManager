#ifndef ITEMLISTWIDGET_H
#define ITEMLISTWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QList>
#include "CustomListView.h"
#include "../core/ItemDetail.h"
#include "../core/ItemAndGroupWrapper.h"
#include "../data/filters/DateFilter.h"
#include "../data/sorters/AbstractSorter.h"

namespace Ui {
class ItemListWidget;
}

class ItemListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ItemListWidget(QWidget *parent = 0);
    ~ItemListWidget();

    /**
     * if new item meets the conditions, add it to correct position
     * @param item
     */
    void addItemDetail(const todo::ItemDetail &item);
    void loadItemDetails(const QList<todo::ItemDetail> &items);  // Attention: will clear pre items first.
    void removeItemDetailByID(const QString& itemID);

    void loadItemWrappers(const QList<todo::ItemAndGroupWrapper> &itemWrappers);

    /**
     * clear condition filters, and release their memory
     */
    void clearFilters();
    template<class T> void addFilter(const T &filter) {
        auto newFilterPtr = dynamic_cast<todo::AbstractFilter*>(new T(filter));
        this->filters.append(newFilterPtr);
    }

    void clearSorters();
    template<class T> void addSorter(const T &sorter) {
        auto newSorterPtr = dynamic_cast<todo::AbstractSorter*>(new T(sorter));
        this->sorters.append(newSorterPtr);
    }

    /**
     * check detail whether meets the condition
     */
    bool checkItem(const todo::ItemDetail &detail);

    /**
     * set cursor to target item according to given itemID and load it to detail widget
     * @param itemID
     */
    void setCurrentItemByID(const QString &itemID);

public slots:
    /**
     * Refresh item info, used then current item is edited so the list view can refresh immediately
     * ONLY REFRESH.
     * @param item: item after edited but has the same item id
     */
    void refresh_item_info(const todo::ItemDetail &item);

    /**
     * Refresh item info.
     * AND MOVE IT TO CORRECT POSITION DUE TO SORTERS
     * ATTENTION: can't use reference here !!!
     * @param item
     */
    void refresh_item_info_and_sort(todo::ItemDetail item);

    /**
     * Same like refresh_item_info. But it will remove item if item can't meet the condition.
     */
    void refresh_or_remove_item_info(const todo::ItemDetail &item);

private slots:
    void listWidget_selectedItem_changed();

signals:
    void selectedItemChanged(const QString &newItemID);

private:
    /**
     * get details that meet the condition in given details
     * @param details
     * @return
     */
    QList<todo::ItemDetail> filtItemList(const QList<todo::ItemDetail> &details);

    QList<todo::ItemAndGroupWrapper> sortItemList(const QList<todo::ItemAndGroupWrapper> &wrapper);

    /**
     * find which row item is at.
     * @param item
     * @return -1 if not exists
     */
    int findRow(const todo::ItemDetail &item);

    int findRowByID(const QString &itemID);

    /**
     * add item to list view without considering whether it meets the conditions or not
     * @param itemWrapper
     */
    void addItemWrapper_(const todo::ItemAndGroupWrapper &itemWrapper);

    todo::ItemAndGroupWrapper getItemPairByRow(int row);
    todo::ItemAndGroupWrapper getCurrSelectedItemPair();

    Ui::ItemListWidget *ui;
    QStandardItemModel *itemModel;
    CustomListView *listView;
    QList<todo::AbstractFilter*> filters;
    QList<todo::AbstractSorter*> sorters;
};

#endif // ITEMLISTWIDGET_H
