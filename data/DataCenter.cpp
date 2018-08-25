//
// Created by septemberhx on 5/10/18.
//

#include <QDebug>
#include "DataCenter.h"
#include "../core/SqlErrorException.h"
#include "../utils/StringUtils.h"

bool compareFunc(const todo::ItemDetailAndTag &t1, const todo::ItemDetailAndTag &t2) {
    if (t1.getItemID() < t2.getItemID()) {
        return true;
    } else if (t1.getItemID() > t2.getItemID()) {
        return false;
    } else {
        if (t1.getOrder() < t2.getOrder()) {
            return true;
        } else {
            return false;
        }
    }
}

QList<todo::ItemDetail> todo::DataCenter::selectItemDetailByDate(const QDate &targetDate) {
    QList<ItemDetailDao> results = DaoFactory::getInstance()->getSQLDao()->selectItemDetailByDate(targetDate);
    return this->fillItemDetailInfo(results);
}

void todo::DataCenter::updateItemDetailByID(const QString &itemID, const ItemDetail &oldItemDetail, const ItemDetail &newItemDetail) {
    DaoFactory::getInstance()->getSQLDao()->startTransaction();
    bool sqlExecResult = true;
    try {
        // main part
        this->updateItemDetailByID_(itemID, oldItemDetail, newItemDetail);
        // end
    } catch (const SqlErrorException &e) {
        sqlExecResult = false;
    }
    DaoFactory::getInstance()->getSQLDao()->endTransaction(sqlExecResult);
    if (sqlExecResult) {
        emit(this->databaseModified());
    }
}

void todo::DataCenter::updateItemDetailsByIDList(const QList<QString> &itemIDList,
                                                 const QList<todo::ItemDetail> &oldItemDetailList,
                                                 const QList<ItemDetail> &newItemDetailList) {
    assert(itemIDList.size() == oldItemDetailList.size());
    assert(itemIDList.size() == newItemDetailList.size());
    DaoFactory::getInstance()->getSQLDao()->startTransaction();
    bool sqlExecResult = true;
    try {
        // main part
        for (int i = 0; i < itemIDList.size(); ++i) {
            this->updateItemDetailByID_(itemIDList[i], oldItemDetailList[i], newItemDetailList[i]);
        }
        //end
    } catch (const SqlErrorException &e) {
        sqlExecResult = false;
    }
    DaoFactory::getInstance()->getSQLDao()->endTransaction(sqlExecResult);
    if (sqlExecResult) {
        emit(this->databaseModified());
    }
}


void todo::DataCenter::updateItemDetailByID_(const QString &itemID, const todo::ItemDetail &oldItemDetail,
                                             const todo::ItemDetail &newItemDetail) {
    if (oldItemDetail.toDao() != newItemDetail.toDao()) {
        DaoFactory::getInstance()->getSQLDao()->updateItemDetailByID(itemID, newItemDetail.toDao());
        GlobalCache::getInstance()->updateItemDetailDaoByID(itemID, newItemDetail.toDao());
    }

    if (newItemDetail.isTagDiff(oldItemDetail)) {
        // For Now
        DaoFactory::getInstance()->getSQLDao()->deleteItemAndTagMatchByItemID(newItemDetail.getId());
        int i = 1;
        for (auto const &tag : newItemDetail.getTags()) {
            DaoFactory::getInstance()->getSQLDao()->insertItemAndTagMatch(
                    ItemDetailAndTag(newItemDetail.getId(), tag.getId(), i));
            ++i;
        }
    }

    if (newItemDetail.isRootGroupDiff(oldItemDetail) || newItemDetail.isDirectGroupDiff(oldItemDetail)) {
        DaoFactory::getInstance()->getSQLDao()->deleteItemGroupRelationByItemID(newItemDetail.getId());
        GlobalCache::getInstance()->deleteRelationByItemID(newItemDetail.getId());
        if (newItemDetail.hasRootGroup()) {
            DaoFactory::getInstance()->getSQLDao()->insertItemGroupRelation(newItemDetail.generateRelation());
            GlobalCache::getInstance()->addRelation(newItemDetail.generateRelation());
        }
    }
}

void todo::DataCenter::deleteItemDetailByIDCompletely(const QString &itemID) {
    DaoFactory::getInstance()->getSQLDao()->startTransaction();
    bool sqlExecResult = true;
    try {
        // main part
        this->deleteItemDetailByIDCompletely_(itemID);
        // end
    } catch (const SqlErrorException &e) {
        sqlExecResult = false;
    }
    DaoFactory::getInstance()->getSQLDao()->endTransaction(sqlExecResult);
    if (sqlExecResult) {
        emit(this->databaseModified());
    }
}

void todo::DataCenter::insertItemDetail(const todo::ItemDetail &itemDetail) {
    DaoFactory::getInstance()->getSQLDao()->startTransaction();
    bool sqlExecResult = true;
    try {
        // main part
        DaoFactory::getInstance()->getSQLDao()->insertItemDetail(itemDetail.toDao());
        int i = 1;
        for (auto const &tag : itemDetail.getTags()) {
            DaoFactory::getInstance()->getSQLDao()->insertItemAndTagMatch(
                    ItemDetailAndTag(itemDetail.getId(), tag.getId(), i));
            ++i;
        }

        if (itemDetail.hasRootGroup()) {
            DaoFactory::getInstance()->getSQLDao()->insertItemGroupRelation(itemDetail.generateRelation());
            GlobalCache::getInstance()->addRelation(itemDetail.generateRelation());
        }
        // end
    } catch (const SqlErrorException &e) {
        sqlExecResult = false;
    }
    DaoFactory::getInstance()->getSQLDao()->endTransaction(sqlExecResult);
    if (sqlExecResult) {
        emit(this->databaseModified());
    }
}

void todo::DataCenter::updateDoneByID(const QString &itemID, bool flag) {
    DaoFactory::getInstance()->getSQLDao()->startTransaction();
    bool sqlExecResult = true;
    try {
        // main part
        DaoFactory::getInstance()->getSQLDao()->updateDoneByID(itemID, flag);
        auto oldCacheItems = GlobalCache::getInstance()->getItemDetailDaoByID(itemID);
        if (oldCacheItems.size() > 0) {
            oldCacheItems[0].setDone(flag);
            GlobalCache::getInstance()->updateItemDetailDaoByID(itemID, oldCacheItems[0]);
        }
        // end
    } catch (const SqlErrorException &e) {
        sqlExecResult = false;
    }
    DaoFactory::getInstance()->getSQLDao()->endTransaction(sqlExecResult);
    if (sqlExecResult) {
        emit(this->databaseModified());
    }
}

QList<todo::ItemTag> todo::DataCenter::selectItemTagById(const QString &tagId) {
    return DaoFactory::getInstance()->getSQLDao()->selectItemTagById(tagId);
}

void todo::DataCenter::updateItemTagById(const QString &tagId, const todo::ItemTag &itemTag) {
    DaoFactory::getInstance()->getSQLDao()->updateItemTagById(tagId, itemTag);
}

void todo::DataCenter::deleteItemTagOnlyById(const QString &tagId) {
    DaoFactory::getInstance()->getSQLDao()->startTransaction();
    bool sqlExecResult = true;
    try {
        // main part
        DaoFactory::getInstance()->getSQLDao()->deleteItemTagById(tagId);
        DaoFactory::getInstance()->getSQLDao()->deleteItemAndTagMatchByTagID(tagId);
        // end
    } catch (const SqlErrorException &e) {
        sqlExecResult = false;
    }
    DaoFactory::getInstance()->getSQLDao()->endTransaction(sqlExecResult);
    if (sqlExecResult) {
        emit(this->databaseModified());
    }
}

void todo::DataCenter::deleteItemTagAndAllItemsByTagId(const QString &tagId) {
    DaoFactory::getInstance()->getSQLDao()->startTransaction();
    bool sqlExecResult = true;
    try {
        // main part
        auto tagMatchList = DaoFactory::getInstance()->getSQLDao()->selectItemAndTagMatchByTagID(tagId);
        QList<QString> itemIDList;
                foreach (auto const &tagMatch, tagMatchList) {
                itemIDList.append(tagMatch.getItemID());
            }
        DaoFactory::getInstance()->getSQLDao()->deleteItemTagById(tagId);
        this->deleteItemDetailByIDsCompletely_(itemIDList);
        // end
    } catch (const SqlErrorException &e) {
        sqlExecResult = false;
    }
    DaoFactory::getInstance()->getSQLDao()->endTransaction(sqlExecResult);
    if (sqlExecResult) {
        emit(this->databaseModified());
    }
}

void todo::DataCenter::insertItemTag(const todo::ItemTag &tag) {
    DaoFactory::getInstance()->getSQLDao()->insertItemTag(tag);
}

QList<todo::ItemTag> todo::DataCenter::selectAllItemTag() {
    return DaoFactory::getInstance()->getSQLDao()->selectAllItemTag();
}

QList<todo::ItemDetail> todo::DataCenter::fillTagInfo(const QList<todo::ItemDetailDao> &itemDetailDaos) {
    QList<QString> itemIDs;
    QList<ItemDetail> resultList;
    for (auto const &itemDao : itemDetailDaos) {
        itemIDs.append(itemDao.getId());
        resultList.append(ItemDetail(itemDao));
    }

    QList<ItemTag> itemTags = this->selectAllItemTag();
    QMap<QString, ItemTag> itemTagMap;
    for (auto const &itemTag : itemTags) {
        itemTagMap.insert(itemTag.getId(), itemTag);
    }

    QList<ItemDetailAndTag> matches = DaoFactory::getInstance()->getSQLDao()->selectItemAndTagMatchByItemIDs(itemIDs);

    std::sort(matches.begin(), matches.end(), compareFunc);
    for (auto const &match : matches) {
        for (auto &item : resultList) {
            if (item.getId() == match.getItemID()) {
                item.addTag(itemTagMap[match.getTagID()]);
            }
        }
    }
    return resultList;
}

QList<todo::ItemDetail> todo::DataCenter::selectNextNotifiedItemDetail() {
    return DaoFactory::getInstance()->getSQLDao()->selectNextNotifiedItemDetail();
}

todo::DataCenter::DataCenter(QObject *parent) : QObject(parent) {

}

QList<todo::ItemDetail> todo::DataCenter::selectItemDetailByDate(const QDate &fromDate, const QDate &toDate) {
    auto results = DaoFactory::getInstance()->getSQLDao()->selectItemDetailByDate(fromDate, toDate);
    return this->fillItemDetailInfo(results);
}

QList<todo::ItemDetail> todo::DataCenter::selectItemDetailsByTag(const todo::ItemTag &itemTag) {
    auto itemAndTagMatchList = DaoFactory::getInstance()->getSQLDao()->selectItemAndTagMatchByTagID(itemTag.getId());
    QList<QString> itemDetailIds;
    for (auto const &itemAndTagMatch : itemAndTagMatchList) {
        itemDetailIds.append(itemAndTagMatch.getItemID());
    }
    auto itemDaos = GlobalCache::getInstance()->getItemDetailDaoByIDs(itemDetailIds);
    return this->fillItemDetailInfo(itemDaos);
}

QList<todo::ItemGroup> todo::DataCenter::selectItemGroupByID(const QString &groupID) {
    auto itemGroupDaos = GlobalCache::getInstance()->getItemGroupDaoByID(groupID);
    return this->fillItemGroupInfo(itemGroupDaos);
}

void todo::DataCenter::updateItemGroupByID(const QString &groupID, const todo::ItemGroup &itemGroup) {
    DaoFactory::getInstance()->getSQLDao()->updateItemGroupByID(groupID, itemGroup.toDao());
    GlobalCache::getInstance()->updateItemGroupDaoByID(groupID, itemGroup.toDao());
    emit databaseModified();
}

QList<todo::ItemGroup> todo::DataCenter::selectItemGroupByIDs(const QList<QString> &groupIDs) {
    auto itemGroupDaoList = GlobalCache::getInstance()->getItemGroupDaoByIDs(groupIDs);
    return this->fillItemGroupInfo(itemGroupDaoList);
}

void todo::DataCenter::insertItemGroup(const todo::ItemGroup &itemGroup) {
    DaoFactory::getInstance()->getSQLDao()->insertItemGroup(itemGroup.toDao());
    GlobalCache::getInstance()->updateItemGroupDaoByID(itemGroup.getId(), itemGroup.toDao());

    if (itemGroup.hasRootGroup()) {
        ItemGroupRelation relation;
        relation.setItemID(itemGroup.getId());
        relation.setRootGroupID(itemGroup.getRootGroupID());
        relation.setDirectGroupID(itemGroup.getDirectGroupID());
        DaoFactory::getInstance()->getSQLDao()->insertItemGroupRelation(relation);
        GlobalCache::getInstance()->addRelation(relation);
    }
}

QList<todo::ItemGroupRelation> todo::DataCenter::selectItemGroupRelationByRootID(const QString &rootID) {
    return GlobalCache::getInstance()->getItemGroupRelationByRootID(rootID);
}

QList<todo::ItemAndGroupWrapper> todo::DataCenter::selectItemByDirectGroupID(const QString &groupID) {
    auto relationList = GlobalCache::getInstance()->getItemGroupRelationByDirectGroupID(groupID);
    QList<QString> itemDetailIDs;
    QList<QString> itemGroupIDs;
    foreach (auto const &relation, relationList) {
        if (todo::StringUtils::checkIfItemDetail(relation.getItemID())) {
            itemDetailIDs.append(relation.getItemID());
        } else if (todo::StringUtils::checkIfItemGroup(relation.getItemID())) {
            itemGroupIDs.append(relation.getItemID());
        }
    }

    auto itemDetailDaoList = GlobalCache::getInstance()->getItemDetailDaoByIDs(itemDetailIDs);
    auto itemGroupDaoList = GlobalCache::getInstance()->getItemGroupDaoByIDs(itemGroupIDs);

    auto itemDetailList = this->fillItemDetailInfo(itemDetailDaoList);
    auto itemGroupList = this->fillItemGroupInfo(itemGroupDaoList);
    QList<todo::ItemAndGroupWrapper> wrapperList;
    foreach (auto const &itemDetail, itemDetailList) {
        wrapperList.append(itemDetail);
    }
    foreach (auto const &itemGroup, itemGroupList) {
        wrapperList.append(itemGroup);
    }
    return wrapperList;
}

QList<todo::ItemGroup> todo::DataCenter::selectItemGroupByType(const todo::ItemGroupType &type) {
    auto itemGroupDaoList = DaoFactory::getInstance()->getSQLDao()->selectItemGroupByType(type);
    return this->fillItemGroupInfo(itemGroupDaoList);
}

void todo::DataCenter::deleteGroupCompletely(const QString &groupID) {
    DaoFactory::getInstance()->getSQLDao()->startTransaction();
    bool sqlExecResult = true;
    try {
        // main part
        this->deleteGroupCompletely_(groupID);
        // end
    } catch (const SqlErrorException &e) {
        sqlExecResult = false;
    }
    DaoFactory::getInstance()->getSQLDao()->endTransaction(sqlExecResult);
    if (sqlExecResult) {
        emit(this->databaseModified());
    }
}

void todo::DataCenter::deleteGroupCompletely_(const QString &groupID) {
    auto relationList = DaoFactory::getInstance()->getSQLDao()->selectItemGroupRelationByParentID(groupID);

    // delete the relation
    // delete child details and child groups
    QList<QString> itemDetailIDs, groupIDs;
    foreach (auto const &relation, relationList) {
        if (todo::StringUtils::checkIfItemGroup(relation.getItemID())) {
            this->deleteGroupCompletely_(relation.getItemID());  // recursive call
        } else if (todo::StringUtils::checkIfItemDetail(relation.getItemID())) {
            this->deleteItemDetailByIDCompletely(relation.getItemID());
        }
        DaoFactory::getInstance()->getSQLDao()->deleteItemGroupRelationByDirectParentIDAndItemID(
                relation.getDirectGroupID(), relation.getItemID()
        );
        GlobalCache::getInstance()->deleteRelationByItemID(relation.getItemID());
    }

    // delete itself
    DaoFactory::getInstance()->getSQLDao()->deleteItemGroupByID(groupID);
    GlobalCache::getInstance()->deleteItemGroupDaoByID(groupID);
}

void todo::DataCenter::deleteItemDetailByIDCompletely_(const QString &itemID) {
    DaoFactory::getInstance()->getSQLDao()->deleteItemDetailByID(itemID);
    DaoFactory::getInstance()->getSQLDao()->deleteItemAndTagMatchByItemID(itemID);
    DaoFactory::getInstance()->getSQLDao()->deleteItemGroupRelationByItemID(itemID);
    GlobalCache::getInstance()->deleteRelationByItemID(itemID);
    GlobalCache::getInstance()->deleteItemDetailDaoByID(itemID);
}

void todo::DataCenter::deleteItemDetailByIDsCompletely_(const QList<QString> &itemIDList) {
    DaoFactory::getInstance()->getSQLDao()->deleteItemDetailByIDs(itemIDList);
    DaoFactory::getInstance()->getSQLDao()->deleteItemAndTagMatchByItemIDs(itemIDList);
    DaoFactory::getInstance()->getSQLDao()->deleteItemGroupRelationByItemIDs(itemIDList);
    GlobalCache::getInstance()->deleteRelationByItemIDs(itemIDList);
    GlobalCache::getInstance()->deleteItemDetailDaoByIDs(itemIDList);
}

QList<todo::ItemGroup> todo::DataCenter::fillItemGroupInfo(const QList<todo::ItemGroupDao> &itemGroupDaos) {
    QList<todo::ItemGroup> resultList;

    foreach (auto const &itemGroupDao, itemGroupDaos) {
        ItemGroup group(itemGroupDao);

        auto relations = GlobalCache::getInstance()->getItemGroupRelationByItemID(group.getId());
        if (relations.size() > 0) {
            group.setRootGroupID(relations[0].getRootGroupID());
            group.setDirectGroupID(relations[0].getDirectGroupID());
        }

        auto otherRelations = GlobalCache::getInstance()->getItemGroupRelationByDirectGroupID(group.getId());
        if (otherRelations.size() > 0) {
            QList<QString> subGroupIDs, subItemIDs;
            foreach (auto const &relation, otherRelations) {
                if (StringUtils::checkIfItemGroup(relation.getItemID())) {
                    subGroupIDs.append(relation.getItemID());
                } else {
                    subItemIDs.append(relation.getItemID());
                }
            }
            group.setSubGroupIDList(subGroupIDs);
            group.setItemDetailIDList(subItemIDs);
        }

        resultList.append(group);
    }

    return resultList;
}

QList<todo::ItemDetail> todo::DataCenter::fillItemDetailInfo(const QList<todo::ItemDetailDao> &itemDetailDaos) {
    auto resultList = this->fillTagInfo(itemDetailDaos);

    for (int i = 0; i < resultList.size(); ++i) {
        auto relations = GlobalCache::getInstance()->getItemGroupRelationByItemID(itemDetailDaos[i].getId());
        if (relations.size() > 0) {
            resultList[i].setRootGroupID(relations[0].getRootGroupID());
            resultList[i].setDirectGroupID(relations[0].getDirectGroupID());
        }
    }

    return resultList;
}

QList<todo::ItemGroupRelation> todo::DataCenter::selectItemGroupRelationByItemID(const QString &itemID) {
    return GlobalCache::getInstance()->getItemGroupRelationByItemID(itemID);
}

QList<todo::ItemGroupRelation> todo::DataCenter::selectAllItemGroupRelation() {
    return DaoFactory::getInstance()->getSQLDao()->selectAllItemGroupRelation();
}

QList<todo::ItemDetail> todo::DataCenter::selectItemDetailByIDs(const QList<QString> &itemIDs) {
    auto itemDetailDaos = GlobalCache::getInstance()->getItemDetailDaoByIDs(itemIDs);
    return this->fillItemDetailInfo(itemDetailDaos);
}

todo::ItemDetail todo::DataCenter::selectItemDetailByID(const QString &itemID) {
    auto itemDetailDaos = GlobalCache::getInstance()->getItemDetailDaoByID(itemID);
    if (itemDetailDaos.size() != 1) {
        qDebug() << itemID;
    }
    assert(itemDetailDaos.size() == 1);
    return this->fillItemDetailInfo(itemDetailDaos)[0];
}

QList<todo::StickyNote> todo::DataCenter::selectStickyNoteByID(const QString &stickyNoteId) {
    auto daoList = DaoFactory::getInstance()->getSQLDao()->selectStickyNoteById(stickyNoteId);
    return this->fillStickyNoteInfo(daoList);
}

void todo::DataCenter::insertStickyNote(const todo::StickyNote &stickyNote) {
    DaoFactory::getInstance()->getSQLDao()->insertStickyNote(stickyNote.toDao());
}

QList<todo::StickyNote> todo::DataCenter::fillStickyNoteInfo(const QList<todo::StickyNoteDao> &stickyNoteDaos) {
    QList<todo::StickyNote> resultList;

    foreach (auto const &dao, stickyNoteDaos) {
        resultList.append(StickyNote(dao));
    }
    return resultList;
}

QList<todo::StickyNote> todo::DataCenter::selectAllStickyNote() {
    auto daoList = DaoFactory::getInstance()->getSQLDao()->selectAllStickyNote();
    return this->fillStickyNoteInfo(daoList);
}

void todo::DataCenter::updateStickyNotePositionById(const QString &id, int x, int y) {
    DaoFactory::getInstance()->getSQLDao()->updateStickyNotePositionById(id, x, y);
}

void todo::DataCenter::updateStickyNote(const todo::StickyNote &stickyNote) {
    DaoFactory::getInstance()->getSQLDao()->updateStickyNoteById(stickyNote.getId(), stickyNote.toDao());
}

void todo::DataCenter::deleteStickyNoteById(const QString &id) {
    DaoFactory::getInstance()->getSQLDao()->deleteStickyNoteById(id);
}

void todo::DataCenter::insertItemDetailTime(const todo::ItemDetailTimeDao &dao) {
    DaoFactory::getInstance()->getSQLDao()->insertItemDetailTime(dao);
}

QList<todo::ItemDetailTimeDao> todo::DataCenter::selectItemDetailTimeByItemID(const QString &itemID) {
    return DaoFactory::getInstance()->getSQLDao()->selectItemDetailTimeByItemID(itemID);
}
