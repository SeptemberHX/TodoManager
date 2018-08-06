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
class ItemAndGroupWrapper {
public:
    ItemAndGroupWrapper();

    ItemAndGroupWrapper(const ItemDetail &itemDetail);

    ItemAndGroupWrapper(const ItemGroup &itemGroup);

    const ItemDetail &getItemDetail() const;

    void setItemDetail(const ItemDetail &itemDetail);

    const ItemGroup &getItemGroup() const;

    void setItemGroup(const ItemGroup &itemGroup);

    bool isGroup() const;

    bool isDetail() const;

    const QString &getID() const;

    QString getRootGroupID() const;

    QString getDirectGroupID() const;

    bool hasRootGroup() const;

    QString getTitle() const;

    QString getDescription() const;

    friend std::ostream &operator<<(std::ostream &os, const ItemAndGroupWrapper &itemAndGroupPair);

    friend QDataStream&operator<<(QDataStream &os, const ItemAndGroupWrapper &itemAndGroupPair);

    friend QDataStream&operator>>(QDataStream &os, ItemAndGroupWrapper &itemAndGroupPair);

private:
    ItemDetail itemDetail;
    ItemGroup itemGroup;
    bool group;
};

inline uint qHash(const ItemAndGroupWrapper &itemAndGroupPair) {
    return qHash(itemAndGroupPair.getItemDetail().getId() + itemAndGroupPair.getItemGroup().getId());
}
}

Q_DECLARE_METATYPE(todo::ItemAndGroupWrapper);

#endif //TODOMANAGER_ITEMANDGROUPPAIR_H
