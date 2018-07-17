//
// Created by septemberhx on 7/17/18.
//

#ifndef TODOMANAGER_ITEMGROUPUTILS_H
#define TODOMANAGER_ITEMGROUPUTILS_H

#include "../core/ItemGroup.h"
#include "../core/ItemDetail.h"
#include "../core/ItemGroupRelation.h"
#include <QList>
#include <QMap>
#include <QString>

namespace todo {

class ItemGroupUtils {
public:
    static QList<ItemGroup> buildGroup(const QList<ItemGroup> &itemGroupList,
                                const QList<ItemDetail> &itemList,
                                const QList<ItemGroupRelation> &relationList
    );

private:
    static void buildGroup_(ItemGroup *rootGroupPtr,
                            const QMap<QString, ItemGroup> &groupMap,
                            const QMap<QString, ItemDetail> &detailMap,
                            const QMap<QString, QList<todo::ItemGroupRelation>> &relationMap
    );
};

}

#endif //TODOMANAGER_ITEMGROUPUTILS_H
