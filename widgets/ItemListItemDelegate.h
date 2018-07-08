//
// Created by septemberhx on 5/8/18.
//

#ifndef TODOMANAGER_ITEMLISTITEMDELEGATE_H
#define TODOMANAGER_ITEMLISTITEMDELEGATE_H

#include <QStyledItemDelegate>

class ItemListItemDelegate : public QStyledItemDelegate {
public:
    ItemListItemDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};


#endif //TODOMANAGER_ITEMLISTITEMDELEGATE_H
