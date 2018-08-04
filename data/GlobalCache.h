//
// Created by septemberhx on 7/31/18.
//

#ifndef TODOMANAGER_GLOBALCACHE_H
#define TODOMANAGER_GLOBALCACHE_H

#include <QObject>
#include <QCache>
#include <QMap>
#include <QString>
#include "../core/ItemGroup.h"
#include "../core/ItemDetailDao.h"
#include "../core/ItemGroupDao.h"
#include "../core/ItemGroupOverview.h"

namespace todo {

class DataCenter;

class GlobalCache : public QObject {
    Q_OBJECT
public:
    static GlobalCache* getInstance();
    const ItemDetailDao& getItemDetailDaoByID(const QString &itemID);
    QList<ItemDetailDao> getItemDetailDaoByIDs(const QList<QString> &itemIDs);
    const ItemGroupDao& getItemGroupDaoByID(const QString &itemID);
    QList<ItemGroupDao> getItemGroupDaoByIDs(const QList<QString> &itemIDs);
    void updateItemDetailDaoByID(const QString &itemID, const ItemDetailDao &detailDao);
    void updateItemGroupDaoByID(const QString &itemID, const ItemGroupDao &groupDao);

    QList<ItemGroupRelation> getItemGroupRelationByDirectGroupID(const QString &directGroupID);
    QList<ItemGroupRelation> getItemGroupRelationByItemID(const QString &itemID);
    QList<ItemGroupRelation> getItemGroupRelationByRootID(const QString &itemID);

    void deleteRelationByItemID(const QString &taskID);
    void addRelation(const ItemGroupRelation &relation);

private:
    static GlobalCache* instance;

    GlobalCache();
    QCache<QString, ItemDetailDao> itemDetailDaoCache;
    QCache<QString, ItemGroupDao> itemGroupDaoCache;

    QList<ItemGroupRelation> relationList;
    QMap<QString, int> itemID2RelationIndex;  // item relation
    QMap<QString, QList<int>> groupID2RelationIndex;  // sub items for group
};

}


#endif //TODOMANAGER_GLOBALCACHE_H
