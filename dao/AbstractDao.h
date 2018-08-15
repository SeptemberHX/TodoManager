//
// Created by septemberhx on 5/16/18.
//

#ifndef TODOMANAGER_ABSTRACTDAO_H
#define TODOMANAGER_ABSTRACTDAO_H

#include <QDate>
#include <QList>
#include <QString>
#include "../core/ItemDetailDao.h"
#include "../core/ItemTag.h"
#include "../core/ItemDetailAndTag.h"
#include "../core/ItemGroup.h"
#include "../core/ItemGroupDao.h"
#include "../core/ItemGroupRelation.h"

namespace todo {
class AbstractDao {
public:
    AbstractDao() = default;
    // ItemDetail
    virtual QList<ItemDetailDao> selectItemDetailByDate(const QDate &targetDate) = 0;
    virtual void updateItemDetailByID(const QString &itemID, const ItemDetailDao &itemDetailDao) = 0;
    virtual void deleteItemDetailByID(const QString &itemID) = 0;
    virtual void deleteItemDetailByIDs(const QList<QString> &itemIDs) = 0;
    virtual void insertItemDetail(const ItemDetailDao &itemDetailDao) = 0;
    virtual void updateDoneByID(const QString &itemID, bool flag) = 0;
    virtual QList<ItemDetailDao> selectItemDetailByDate(const QDate &fromDate, const QDate &toDate) = 0;
    virtual QList<ItemDetailDao> selectItemDetailByIDs(const QList<QString> &itemIDs) = 0;
    // End

    // ItemTag
    virtual QList<ItemTag> selectAllItemTag() = 0;
    virtual QList<ItemTag> selectItemTagById(const QString &tagId) = 0;
    virtual void updateItemTagById(const QString &tagId, const ItemTag &itemTag) = 0;
    virtual void deleteItemTagById(const QString &tagId) = 0;
    virtual void insertItemTag(const ItemTag &tag) = 0;
    // End

    // ItemDetailAndTag
    virtual QList<ItemDetailAndTag> selectItemAndTagMatchByItemID(const QString &itemID) = 0;
    virtual QList<ItemDetailAndTag> selectItemAndTagMatchByItemIDs(const QList<QString> &itemIDs) = 0;
    virtual QList<ItemDetailAndTag> selectItemAndTagMatchByTagID(const QString &tagID) = 0;
    virtual QList<ItemDetailAndTag> selectItemAndTagMatchByTagIDs(const QList<QString> &tagIDs) = 0;
    virtual void insertItemAndTagMatch(const ItemDetailAndTag &newMatch) = 0;
    virtual void deleteItemAndTagMatch(const ItemDetailAndTag &match) = 0;
    virtual void deleteItemAndTagMatchByItemID(const QString &itemID) = 0;
    virtual void deleteItemAndTagMatchByItemIDs(const QList<QString> &itemIDList) = 0;
    virtual void deleteItemAndTagMatchByTagID(const QString &tagID) = 0;
    // End

    // For notification
    virtual QList<ItemDetail> selectNextNotifiedItemDetail() = 0;
    // End

    // ItemGroup
    virtual QList<ItemGroupDao> selectItemGroupByID(const QString &groupID) = 0;
    virtual QList<ItemGroupDao> selectItemGroupByType(const todo::ItemGroupType &type) = 0;
    virtual void updateItemGroupByID(const QString &groupID, const ItemGroupDao &itemGroupDao) = 0;
    virtual void deleteItemGroupByID(const QString &groupID) = 0;
    virtual void deleteItemGroupByIDs(const QList<QString> &groupIDList) = 0;
    virtual void insertItemGroup(const ItemGroupDao &itemGroupDao) = 0;
    virtual QList<ItemGroupDao> selectItemGroupByIDs(const QList<QString> &groupIDs) = 0;
    // End

    // ItemGroupRelation
    virtual QList<ItemGroupRelation> selectItemGroupRelationByRootID(const QString &rootID) = 0;
    virtual void deleteItemGroupRelationByDirectParentIDAndItemID(const QString &directParentID, const QString &itemID) = 0;
    virtual void deleteItemGroupRelationByRootID(const QString &rootID) = 0;
    virtual void deleteItemGroupRelationByItemID(const QString &itemID) = 0;
    virtual void deleteItemGroupRelationByItemIDs(const QList<QString> &itemIDList) = 0;
    virtual void insertItemGroupRelation(const ItemGroupRelation &relation) = 0;
    virtual QList<ItemGroupRelation> selectItemGroupRelationByParentID(const QString &parentID) = 0;
    virtual QList<ItemGroupRelation> selectItemGroupRelationByItemID(const QString &itemID) = 0;
    virtual QList<ItemGroupRelation> selectAllItemGroupRelation() = 0;
    // End

    // for transaction
    virtual void startTransaction() = 0;
    virtual void endTransaction(bool isSuccessful) = 0;
    // end

    virtual void init() = 0;
    virtual ~AbstractDao() = default;
};
}

#endif //TODOMANAGER_ABSTRACTDAO_H
