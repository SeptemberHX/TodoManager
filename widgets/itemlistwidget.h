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
    void addItemDetail(const todo::ItemDetail &item);
    void loadItemDetails(const QList<todo::ItemDetail> &items);  // Attention: will clear pre items first.
    void removeItemDetailByID(const QString& itemID);
    void clearFilters();
    void addDateFilter(const QDate &date);

public slots:
    /**
     * Refresh item info, used then current item is edited so the list view can refresh immediately
     * @param item: item after edited but has the same item id
     */
    void refresh_item_info(const todo::ItemDetail &item);

private slots:
    void listWidget_selectedItem_changed();

signals:
    void selectedItemChanged(const QString &newItemID);

private:
    Ui::ItemListWidget *ui;
    QStandardItemModel *itemModel;
    QList<todo::AbstractFilter*> filters;
};

#endif // ITEMLISTWIDGET_H
