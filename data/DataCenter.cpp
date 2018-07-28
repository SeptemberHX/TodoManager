//
// Created by septemberhx on 5/10/18.
//

#include <QDebug>
#include "DataCenter.h"
#include "../utils/ItemUtils.h"

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
    QList<ItemDetail> results = DaoFactory::getInstance()->getSQLDao()->selectItemDetailByDate(targetDate);
    this->fillTagInfo(results);
    return results;
}

void todo::DataCenter::updateItemDetailByID(const QString &itemID, const todo::ItemDetail &itemDetail) {
    DaoFactory::getInstance()->getSQLDao()->updateItemDetailByID(itemID, itemDetail);

    // For Now
    DaoFactory::getInstance()->getSQLDao()->deleteItemAndTagMatchByItemID(itemDetail.getId());
    int i = 1;
    for (auto const &tag : itemDetail.getTags()) {
        DaoFactory::getInstance()->getSQLDao()->insertItemAndTagMatch(ItemDetailAndTag(itemDetail.getId(), tag.getId(), i));
        ++i;
    }

    emit(this->databaseModified());
}

void todo::DataCenter::deleteItemDetailByIDCompletely(const QString &itemID) {
    this->deleteItemDetailByIDCompletely_(itemID);

    emit(this->databaseModified());
}

void todo::DataCenter::insertItemDetail(const todo::ItemDetail &itemDetail) {
    DaoFactory::getInstance()->getSQLDao()->insertItemDetail(itemDetail);
    int i = 1;
    for (auto const &tag : itemDetail.getTags()) {
        DaoFactory::getInstance()->getSQLDao()->insertItemAndTagMatch(ItemDetailAndTag(itemDetail.getId(), tag.getId(), i));
        ++i;
    }

    emit(this->databaseModified());
}

void todo::DataCenter::updateDoneByID(const QString &itemID, bool flag) {
    DaoFactory::getInstance()->getSQLDao()->updateDoneByID(itemID, flag);

    emit(this->databaseModified());
}

QList<todo::ItemTag> todo::DataCenter::selectItemTagById(const QString &tagId) {
    return DaoFactory::getInstance()->getSQLDao()->selectItemTagById(tagId);
}

void todo::DataCenter::updateItemTagById(const QString &tagId, const todo::ItemTag &itemTag) {
    DaoFactory::getInstance()->getSQLDao()->updateItemTagById(tagId, itemTag);
}

void todo::DataCenter::deleteItemTagById(const QString &tagId) {
    DaoFactory::getInstance()->getSQLDao()->deleteItemTagById(tagId);
    emit databaseModified();
}

void todo::DataCenter::insertItemTag(const todo::ItemTag &tag) {
    DaoFactory::getInstance()->getSQLDao()->insertItemTag(tag);
}

QList<todo::ItemTag> todo::DataCenter::selectAllItemTag() {
    return DaoFactory::getInstance()->getSQLDao()->selectAllItemTag();
}

void todo::DataCenter::fillTagInfo(QList<todo::ItemDetail> &itemDetails) {
    QList<QString> itemIDs;
    for (auto const &item : itemDetails) {
        itemIDs.append(item.getId());
    }

    QList<ItemTag> itemTags = this->selectAllItemTag();
    QMap<QString, ItemTag> itemTagMap;
    for (auto const &itemTag : itemTags) {
        itemTagMap.insert(itemTag.getId(), itemTag);
    }

    QList<ItemDetailAndTag> matches = DaoFactory::getInstance()->getSQLDao()->selectItemAndTagMatchByItemIDs(itemIDs);

    std::sort(matches.begin(), matches.end(), compareFunc);
    for (auto const &match : matches) {
        for (auto &item : itemDetails) {
            if (item.getId() == match.getItemID()) {
                item.addTag(itemTagMap[match.getTagID()]);
            }
        }
    }
}

QList<todo::ItemDetail> todo::DataCenter::selectNextNotifiedItemDetail() {
    return DaoFactory::getInstance()->getSQLDao()->selectNextNotifiedItemDetail();
}

todo::DataCenter::DataCenter(QObject *parent) : QObject(parent) {

}

QList<todo::ItemDetail> todo::DataCenter::selectItemDetailByDate(const QDate &fromDate, const QDate &toDate) {
    auto results = DaoFactory::getInstance()->getSQLDao()->selectItemDetailByDate(fromDate, toDate);
    this->fillTagInfo(results);
    return results;
}

QList<todo::ItemDetail> todo::DataCenter::selectItemDetailsByTag(const todo::ItemTag &itemTag) {
    auto itemAndTagMatchList = DaoFactory::getInstance()->getSQLDao()->selectItemAndTagMatchByTagID(itemTag.getId());
    QList<QString> itemDetailIds;
    for (auto const &itemAndTagMatch : itemAndTagMatchList) {
        itemDetailIds.append(itemAndTagMatch.getItemID());
    }
    QList<ItemDetail> itemDetails = DaoFactory::getInstance()->getSQLDao()->selectItemDetailByIDs(itemDetailIds);
    this->fillTagInfo(itemDetails);
    return itemDetails;
}

QList<todo::ItemGroup> todo::DataCenter::selectItemGroupByID(const QString &groupID) {
    return DaoFactory::getInstance()->getSQLDao()->selectItemGroupByID(groupID);
}

void todo::DataCenter::updateItemGroupByID(const QString &groupID, const todo::ItemGroup &itemGroup) {
    DaoFactory::getInstance()->getSQLDao()->updateItemGroupByID(groupID, itemGroup);
}

void todo::DataCenter::deleteItemGroupByID(const QString &groupID) {
    DaoFactory::getInstance()->getSQLDao()->deleteItemGroupByID(groupID);
    emit databaseModified();
}

QList<todo::ItemGroup> todo::DataCenter::selectItemGroupByIDs(const QList<QString> &groupIDs) {
    return DaoFactory::getInstance()->getSQLDao()->selectItemGroupByIDs(groupIDs);
}

void todo::DataCenter::insertItemGroup(const todo::ItemGroup &itemGroup) {
    DaoFactory::getInstance()->getSQLDao()->insertItemGroup(itemGroup);
}

QList<todo::ItemGroupRelation> todo::DataCenter::selectItemGroupRelationByRootID(const QString &rootID) {
    return DaoFactory::getInstance()->getSQLDao()->selectItemGroupRelationByRootID(rootID);
}

void todo::DataCenter::deleteItemGroupRelationByDirectParentIDAndItemID(const QString &directParentID,
                                                                        const QString &itemID) {
    DaoFactory::getInstance()->getSQLDao()->deleteItemGroupRelationByDirectParentIDAndItemID(directParentID, itemID);
    emit databaseModified();
}

void todo::DataCenter::insertItemGroupRelation(const todo::ItemGroupRelation &relation) {
    DaoFactory::getInstance()->getSQLDao()->insertItemGroupRelation(relation);
}

QList<todo::ItemAndGroupWrapper> todo::DataCenter::selectItemByDirectGroupID(const QString &groupID) {
    auto relationList = DaoFactory::getInstance()->getSQLDao()->selectItemGroupRelationByParentID(groupID);
    QList<QString> itemDetailIDs;
    QList<QString> itemGroupIDs;
    foreach (auto const &relation, relationList) {
        if (todo::ItemUtils::checkIfItemDetail(relation.getItemID())) {
            itemDetailIDs.append(relation.getItemID());
        } else if (todo::ItemUtils::checkIfItemGroup(relation.getItemID())) {
            itemGroupIDs.append(relation.getItemID());
        }
    }

    auto itemDetailList = DaoFactory::getInstance()->getSQLDao()->selectItemDetailByIDs(itemDetailIDs);
    auto itemGroupList = DaoFactory::getInstance()->getSQLDao()->selectItemGroupByIDs(itemGroupIDs);
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
    return DaoFactory::getInstance()->getSQLDao()->selectItemGroupByType(type);
}

void todo::DataCenter::deleteGroupCompletely(const QString &groupID) {
    this->deleteGroupCompletely_(groupID);

    // emit signal
    emit databaseModified();
}

void todo::DataCenter::deleteGroupCompletely_(const QString &groupID) {
    auto relationList = DaoFactory::getInstance()->getSQLDao()->selectItemGroupRelationByParentID(groupID);

    // delete the relation
    // delete child details and child groups
    QList<QString> itemDetailIDs, groupIDs;
    foreach (auto const &relation, relationList) {
        if (todo::ItemUtils::checkIfItemGroup(relation.getItemID())) {
            this->deleteGroupCompletely_(relation.getItemID());  // recursive call
        } else if (todo::ItemUtils::checkIfItemDetail(relation.getItemID())) {
            this->deleteItemDetailByIDCompletely(relation.getItemID());
        }
        DaoFactory::getInstance()->getSQLDao()->deleteItemGroupRelationByDirectParentIDAndItemID(
                relation.getDirectGroupID(), relation.getItemID()
        );
    }

    // delete itself
    this->deleteItemGroupByID(groupID);
}

void todo::DataCenter::deleteItemDetailByIDCompletely_(const QString &itemID) {
    DaoFactory::getInstance()->getSQLDao()->deleteItemDetailByID(itemID);
    DaoFactory::getInstance()->getSQLDao()->deleteItemAndTagMatchByItemID(itemID);
}
