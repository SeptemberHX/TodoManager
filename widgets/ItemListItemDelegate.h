//
// Created by septemberhx on 5/8/18.
//

#ifndef TODOMANAGER_ITEMLISTITEMDELEGATE_H
#define TODOMANAGER_ITEMLISTITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "../core/ItemDetail.h"
#include "../core/ItemAndGroupWrapper.h"


class ItemListItemDelegate : public QStyledItemDelegate {
public:
    ItemListItemDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void paintItemDetail(const todo::ItemDetail &itemDetail, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void paintItemGroup(const todo::ItemGroup &itemGroup, QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    static int getDefaultWidth();

private:
    int arcLength;
    QFont titleFont;
    QFont descriptionFont;
    QFont infoFont;
    QFont tagFont;

    QMargins titleMargins;
    QMargins tagGroupMargins;
    QMargins infoMargins;
    QMargins descriptionMargins;

    int percentWidth;
    int percentHeight;
};


#endif //TODOMANAGER_ITEMLISTITEMDELEGATE_H
