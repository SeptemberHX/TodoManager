//
// Created by septemberhx on 7/31/18.
//

#include "../dao/DaoFactory.h"
#include "GlobalCache.h"
#include <QDebug>

todo::GlobalCache* todo::GlobalCache::instance = nullptr;

todo::GlobalCache::GlobalCache() :
    itemDetailDaoCache(5000),
    itemGroupDaoCache(5000)
{
}

todo::GlobalCache *todo::GlobalCache::getInstance() {
    if (GlobalCache::instance == nullptr) {
        GlobalCache::instance = new GlobalCache();
    }
    return GlobalCache::instance;
}

const todo::ItemDetailDao &todo::GlobalCache::getItemDetailDaoByID(const QString &itemID) {
    if (!this->itemDetailDaoCache.contains(itemID)) {
        auto targetDaos = DaoFactory::getInstance()->getSQLDao()->selectItemDetailByIDs({itemID});
        auto targetDaoPtr = new ItemDetailDao(targetDaos[0]);
        this->itemDetailDaoCache.insert(itemID, targetDaoPtr);
    }
    return *(this->itemDetailDaoCache[itemID]);
}

const todo::ItemGroupDao &todo::GlobalCache::getItemGroupDaoByID(const QString &itemID) {
    if (!this->itemGroupDaoCache.contains(itemID)) {
        auto targetDaos = DaoFactory::getInstance()->getSQLDao()->selectItemGroupByID(itemID);
        auto targetDaoPtr = new ItemGroupDao(targetDaos[0]);
        this->itemGroupDaoCache.insert(itemID, targetDaoPtr);
    }
    return *(this->itemGroupDaoCache[itemID]);
}

void todo::GlobalCache::updateItemDetailDaoByID(const QString &itemID, const todo::ItemDetailDao& detailDao) {
    this->itemDetailDaoCache.insert(itemID, new ItemDetailDao(detailDao));
}

void todo::GlobalCache::updateItemGroupDaoByID(const QString &itemID, const todo::ItemGroupDao &groupDao) {
    this->itemGroupDaoCache.insert(itemID, new ItemGroupDao(groupDao));
}

QList<todo::ItemDetailDao> todo::GlobalCache::getItemDetailDaoByIDs(const QList<QString> &itemIDs) {
    if (itemIDs.empty()) return {};

    QList<QString> notInCacheIDs;
    QMap<int, ItemDetailDao> indexDaoMap;  // keep the order
    for (int i = 0; i < itemIDs.size(); ++i) {
        if (this->itemDetailDaoCache.contains(itemIDs[i])) {
            indexDaoMap.insert(i, *this->itemDetailDaoCache[itemIDs[i]]);
        } else {
            notInCacheIDs.append(itemIDs[i]);
        }
    }
    qDebug() << "cache hit size " << indexDaoMap.size();
    auto queryDaos = DaoFactory::getInstance()->getSQLDao()->selectItemDetailByIDs(notInCacheIDs);
    foreach (auto const &itemDao, queryDaos) {
        this->itemDetailDaoCache.insert(itemDao.getId(), new ItemDetailDao(itemDao));
    }

    QList<ItemDetailDao> resultList;
    int floatIndex = 0;
    for (int i = 0; i < itemIDs.size(); ++i) {
        if (indexDaoMap.contains(i)) {
            resultList.append(indexDaoMap[i]);
        } else {
            resultList.append(queryDaos[floatIndex++]);
        }
    }
    return resultList;
}

QList<todo::ItemGroupDao> todo::GlobalCache::getItemGroupDaoByIDs(const QList<QString> &itemIDs) {
    if (itemIDs.empty()) return {};

    QList<QString> notInCacheIDs;
    QMap<int, ItemGroupDao> indexDaoMap;  // keep the order
    for (int i = 0; i < itemIDs.size(); ++i) {
        if (this->itemGroupDaoCache.contains(itemIDs[i])) {
            indexDaoMap.insert(i, *this->itemGroupDaoCache[itemIDs[i]]);
        } else {
            notInCacheIDs.append(itemIDs[i]);
        }
    }
    qDebug() << "cache hit size " << indexDaoMap.size();
    auto queryDaos = DaoFactory::getInstance()->getSQLDao()->selectItemGroupByIDs(notInCacheIDs);
    foreach (auto const &itemDao, queryDaos) {
        this->itemGroupDaoCache.insert(itemDao.getId(), new ItemGroupDao(itemDao));
    }

    QList<ItemGroupDao> resultList;
    int floatIndex = 0;
    for (int i = 0; i < itemIDs.size(); ++i) {
        if (indexDaoMap.contains(i)) {
            resultList.append(indexDaoMap[i]);
        } else {
            resultList.append(queryDaos[floatIndex++]);
        }
    }
    return resultList;
}
