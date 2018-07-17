//
// Created by septemberhx on 7/17/18.
//

#ifndef TODOMANAGER_ITEMANDGROUPPAIR_H
#define TODOMANAGER_ITEMANDGROUPPAIR_H

#include "ItemDetail.h"
#include "ItemGroup.h"
#include <QDataStream>
#include <QMetaType>

namespace todo {

/**
 * For list view delegate usage.
 */
class ItemAndGroupPair {
public:
    ItemAndGroupPair();

    explicit ItemAndGroupPair(const ItemDetail &itemDetail);

    explicit ItemAndGroupPair(const ItemGroup &itemGroup);

    const ItemDetail &getItemDetail() const;

    void setItemDetail(const ItemDetail &itemDetail);

    const ItemGroup &getItemGroup() const;

    void setItemGroup(const ItemGroup &itemGroup);

    bool isGroup() const;

    void setGroup(bool group);

    const QString &getID() const;

    friend std::ostream &operator<<(std::ostream &os, const ItemAndGroupPair &itemAndGroupPair);

    friend QDataStream&operator<<(QDataStream &os, const ItemAndGroupPair &itemAndGroupPair);

    friend QDataStream&operator>>(QDataStream &os, ItemAndGroupPair &itemAndGroupPair);

private:
    ItemDetail itemDetail;
    ItemGroup itemGroup;
    bool group;
};

inline uint qHash(const ItemAndGroupPair &itemAndGroupPair) {
    return qHash(itemAndGroupPair.getItemDetail().getId() + itemAndGroupPair.getItemGroup().getId());
}
}

Q_DECLARE_METATYPE(todo::ItemAndGroupPair);

#endif //TODOMANAGER_ITEMANDGROUPPAIR_H
