//
// Created by septemberhx on 7/17/18.
//

#ifndef TODOMANAGER_ITEMGROUPUTILS_H
#define TODOMANAGER_ITEMGROUPUTILS_H

#include "../core/ItemGroup.h"
#include "../core/ItemDetail.h"
#include "../core/ItemGroupRelation.h"
#include "../core/ItemGroupOverview.h"
#include "../data/DataCenter.h"
#include <QList>
#include <QMap>
#include <QString>

namespace todo {

class ItemGroupUtils {
public:
    static ItemGroupOverview getGroupOverview(const ItemGroup &itemGroup);

    /**
     * Get the path to subGroup from rootGroup
     * @param subGroupID
     * @return: first element will be rootGroupID, and given subGroupID will at the last.
     */
    static QList<QString> getGroupPath(const QString &subGroupID);

    static QMap<QString, QList<QString>> getFullRelationTree();

private:
    static DataCenter dataCenter;
};

}

#endif //TODOMANAGER_ITEMGROUPUTILS_H
