//
// Created by septemberhx on 7/31/18.
//

#include "../dao/DaoFactory.h"
#include "GlobalCache.h"
#include "../utils/StringUtils.h"
#include <QDebug>
#include <QQueue>

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

QList<todo::ItemDetailDao> todo::GlobalCache::getItemDetailDaoByID(const QString &itemID) {
    QList<todo::ItemDetailDao> resultList;
    if (!this->itemDetailDaoCache.contains(itemID)) {
        auto targetDaos = DaoFactory::getInstance()->getSQLDao()->selectItemDetailByIDs({itemID});
        if (targetDaos.size() > 0) {
            auto targetDaoPtr = new ItemDetailDao(targetDaos[0]);
            resultList.append(*targetDaoPtr);
            this->itemDetailDaoCache.insert(itemID, targetDaoPtr);
        }
    } else {
        resultList.append(*(this->itemDetailDaoCache[itemID]));
    }
    return resultList;
}

QList<todo::ItemGroupDao> todo::GlobalCache::getItemGroupDaoByID(const QString &itemID) {
    QList<todo::ItemGroupDao> resultList;
    if (!this->itemGroupDaoCache.contains(itemID)) {
        auto targetDaos = DaoFactory::getInstance()->getSQLDao()->selectItemGroupByID(itemID);
        if (targetDaos.size() > 0) {
            auto targetDaoPtr = new ItemGroupDao(targetDaos[0]);
            resultList.append(*targetDaoPtr);
            this->itemGroupDaoCache.insert(itemID, targetDaoPtr);
        }
    } else {
        resultList.append(*(this->itemGroupDaoCache[itemID]));
    }

    return resultList;
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
    auto queryDaos = DaoFactory::getInstance()->getSQLDao()->selectItemDetailByIDs(notInCacheIDs);
    assert(queryDaos.size() + indexDaoMap.size() == itemIDs.size());

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

QList<todo::ItemGroupRelation> todo::GlobalCache::getItemGroupRelationByDirectGroupID(const QString &directGroupID) {
    QList<todo::ItemGroupRelation> resultList;
    if (this->groupID2RelationIndex.contains(directGroupID)) {
        foreach (auto const &index, this->groupID2RelationIndex[directGroupID]) {
            resultList.append(this->relationList[index]);
        }
    } else {
        resultList = DaoFactory::getInstance()->getSQLDao()->selectItemGroupRelationByParentID(directGroupID);
        QList<int> indexList;
        foreach (auto const &relation, resultList) {
            int itemIndex = -1;
            if (this->itemID2RelationIndex.contains(relation.getItemID())) {
                itemIndex = this->itemID2RelationIndex[relation.getItemID()];
            } else {
                this->relationList.append(relation);
                itemIndex = this->relationList.size() - 1;
            }

            this->itemID2RelationIndex.insert(relation.getItemID(), itemIndex);
            indexList.append(itemIndex);
        }
        this->groupID2RelationIndex.insert(directGroupID, indexList);
    }
    return resultList;
}

QList<todo::ItemGroupRelation> todo::GlobalCache::getItemGroupRelationByItemID(const QString &itemID) {
    QList<ItemGroupRelation> resultList;
    if (this->itemID2RelationIndex.contains(itemID)) {
        resultList.append(this->relationList[this->itemID2RelationIndex[itemID]]);
    } else {
        auto relations = DaoFactory::getInstance()->getSQLDao()->selectItemGroupRelationByItemID(itemID);
        if (!relations.isEmpty()) {
            resultList.append(relations[0]);
            this->relationList.append(relations[0]);
            this->itemID2RelationIndex.insert(itemID, this->relationList.size() - 1);
        }
    }
    return resultList;
}

QList<todo::ItemGroupRelation> todo::GlobalCache::getItemGroupRelationByRootID(const QString &itemID) {
    QList<ItemGroupRelation> resultList;
    if (this->groupID2RelationIndex.contains(itemID)) {
        QQueue<QString> itemQueue;
        itemQueue.push_back(itemID);
        while (!itemQueue.isEmpty()) {
            QString currItemID = itemQueue.first();
            if (StringUtils::checkIfItemGroup(currItemID)) {
                auto relations = this->getItemGroupRelationByDirectGroupID(currItemID);
                resultList.append(relations);
                foreach (auto const &relation, relations) {
                    itemQueue.append(relation.getItemID());
                }
            } else {
                auto relations = this->getItemGroupRelationByItemID(currItemID);
                resultList.append(relations);
            }

            itemQueue.pop_front();
        }
    } else {
        resultList = DaoFactory::getInstance()->getSQLDao()->selectItemGroupRelationByRootID(itemID);
        foreach (auto const &relation, resultList) {
            if (this->itemID2RelationIndex.contains(relation.getItemID())) {
                continue;
            } else {
                this->relationList.append(relation);
                this->itemID2RelationIndex.insert(relation.getItemID(), this->relationList.size() - 1);
                if (!this->groupID2RelationIndex.contains(relation.getDirectGroupID())) {
                    this->groupID2RelationIndex.insert(relation.getDirectGroupID(), {});
                }
                this->groupID2RelationIndex[relation.getDirectGroupID()].append(this->relationList.size() - 1);
            }
        }
    }
    return resultList;
}

void todo::GlobalCache::deleteRelationByItemID(const QString &taskID) {
    if (!this->itemID2RelationIndex.contains(taskID)) {
        return;
    }

    int relationIndex = this->itemID2RelationIndex[taskID];
    this->itemID2RelationIndex.remove(taskID);
    this->groupID2RelationIndex[this->relationList[relationIndex].getDirectGroupID()].removeOne(relationIndex);
}

void todo::GlobalCache::addRelation(const todo::ItemGroupRelation &relation) {
    this->relationList.append(relation);
    this->itemID2RelationIndex.insert(relation.getItemID(), this->relationList.size() - 1);
    if (!this->groupID2RelationIndex.contains(relation.getDirectGroupID())) {
        this->groupID2RelationIndex.insert(relation.getDirectGroupID(), {});
    }
    this->groupID2RelationIndex[relation.getDirectGroupID()].append(this->relationList.size() - 1);
}

void todo::GlobalCache::deleteRelationByItemIDs(const QList<QString> &taskIDList) {
    foreach (auto const &taskID, taskIDList) {
        this->deleteRelationByItemID(taskID);
    }
}

void todo::GlobalCache::deleteItemDetailDaoByID(const QString &itemID) {
    this->itemDetailDaoCache.remove(itemID);
}

void todo::GlobalCache::deleteItemGroupDaoByID(const QString &itemID) {
    this->itemGroupDaoCache.remove(itemID);
}
