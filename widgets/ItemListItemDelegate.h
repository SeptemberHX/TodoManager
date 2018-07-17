//
// Created by septemberhx on 5/8/18.
//

#ifndef TODOMANAGER_ITEMLISTITEMDELEGATE_H
#define TODOMANAGER_ITEMLISTITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "../core/ItemDetail.h"
#include "../core/ItemAndGroupPair.h"


class ItemListItemDelegate : public QStyledItemDelegate {
public:
    ItemListItemDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void paintItemDetail(const todo::ItemDetail &itemDetail, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};


#endif //TODOMANAGER_ITEMLISTITEMDELEGATE_H
