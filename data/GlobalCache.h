//
// Created by septemberhx on 7/31/18.
//

#ifndef TODOMANAGER_GLOBALCACHE_H
#define TODOMANAGER_GLOBALCACHE_H

#include <QObject>
#include <QCache>
#include "../core/ItemDetailDao.h"
#include "../core/ItemGroupDao.h"

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
private:
    static GlobalCache* instance;

    GlobalCache();
    QCache<QString, ItemDetailDao> itemDetailDaoCache;
    QCache<QString, ItemGroupDao> itemGroupDaoCache;
};

}


#endif //TODOMANAGER_GLOBALCACHE_H
