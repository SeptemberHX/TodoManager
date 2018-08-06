//
// Created by septemberhx on 7/27/18.
//

#ifndef TODOMANAGER_ITEMUTILS_H
#define TODOMANAGER_ITEMUTILS_H

#include <QString>
#include "../core/ItemDetail.h"
#include "../core/ItemAndGroupWrapper.h"
#include "../data/DataCenter.h"

namespace todo {

class ItemUtils {
public:

    static QColor getRootGroupColor(const ItemAndGroupWrapper &wrapper);

    static ItemDetail generateNewItemDetail(const QString &title,
            const QString &rootItemID="", const QString &directItemID="");

    static ItemDetail assignItemDetailToGroup(const ItemDetail &itemDetail, const QString &rGroupID, const QString &dGroupID);
    static QList<ItemDetail> assignItemDetailsToGroup(const QList<ItemDetail> &itemDetails, const QString &rGroupID, const QString &dGroupID);

    static QString getPlainDescription(const ItemAndGroupWrapper &wrapper);
    static QString generateToolTip(const ItemAndGroupWrapper & wrapper);

private:
    static DataCenter dataCenter;

    static void assignItemDetailToGroup(const QString &itemID, const QString &rootGroupID, const QString &directGroupID);
    static void assignItemDetailsToGroup(const QList<QString> &itemIDs, const QString &rootGroupID, const QString &directGroupID);
};

}


#endif //TODOMANAGER_ITEMUTILS_H
