#ifndef ITEMLISTWIDGET_H
#define ITEMLISTWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QList>
#include "../core/ItemDetail.h"
#include "../data/filters/DateFilter.h"
#include "itemlistitemwidget.h"

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
     * if new item meets the conditions, add it
     * @param item
     */
    void addItemDetail(const todo::ItemDetail &item);
    void loadItemDetails(const QList<todo::ItemDetail> &items);  // Attention: will clear pre items first.
    void removeItemDetailByID(const QString& itemID);

    /**
     * clear condition filters, and release their memory
     */
    void clearFilters();
    template<class T> void addFilter(const T &filter) {
        auto newFilterPtr = dynamic_cast<todo::AbstractFilter*>(new T(filter));
        this->filters.append(newFilterPtr);
    }

    /**
     * check detail whether meets the condition
     */
    bool checkItem(const todo::ItemDetail &detail);

public slots:
    /**
     * Refresh item info, used then current item is edited so the list view can refresh immediately
     * ONLY REFRESH.
     * @param item: item after edited but has the same item id
     */
    void refresh_item_info(const todo::ItemDetail &item);

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

    /**
     * add item to list view without considering whether it meets the conditions or not
     * @param item
     */
    void addItemDetail_(const todo::ItemDetail &item);

    Ui::ItemListWidget *ui;
    QStandardItemModel *itemModel;
    QList<todo::AbstractFilter*> filters;
};

#endif // ITEMLISTWIDGET_H
