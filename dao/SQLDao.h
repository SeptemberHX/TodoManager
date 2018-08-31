//
// Created by septemberhx on 5/17/18.
//

#ifndef TODOMANAGER_MYSQLDAO_H
#define TODOMANAGER_MYSQLDAO_H

#include <QSqlDatabase>
#include "AbstractDao.h"
#include "../core/ItemDetailDao.h"
#include "../core/ItemGroupDao.h"

namespace todo {

class SQLDao : public AbstractDao {
public:
    SQLDao();

    SQLDao(int type);

    // ItemDetail
    QList<ItemDetailDao> selectItemDetailByDate(const QDate &targetDate) override;

    void updateItemDetailByID(const QString &itemID, const ItemDetailDao &itemDetailDao) override;

    void deleteItemDetailByID(const QString &itemID) override;

    void deleteItemDetailByIDs(const QList<QString> &itemIDs) override;

    void insertItemDetail(const ItemDetailDao &itemDetailDao) override;

    void updateDoneByID(const QString &itemID, bool flag) override;

    QList<ItemDetailDao> selectItemDetailByDate(const QDate &fromDate, const QDate &toDate) override;

    QList<ItemDetailDao> selectItemDetailByIDs(const QList<QString> &itemIDs) override;
    // End

    // ItemTag
    QList<ItemTag> selectAllItemTag() override;

    QList<ItemTag> selectItemTagById(const QString &tagId) override;

    void updateItemTagById(const QString &tagId, const ItemTag &itemTag) override;

    void deleteItemTagById(const QString &tagId) override;

    void insertItemTag(const ItemTag &tag) override;
    // End

    // ItemDetailAndTag
    QList<ItemDetailAndTag> selectItemAndTagMatchByItemID(const QString &itemID) override;

    QList<ItemDetailAndTag> selectItemAndTagMatchByItemIDs(const QList<QString> &itemIDs) override;

    QList<ItemDetailAndTag> selectItemAndTagMatchByTagID(const QString &tagID) override;

    QList<ItemDetailAndTag> selectItemAndTagMatchByTagIDs(const QList<QString> &tagIDs) override;

    void insertItemAndTagMatch(const ItemDetailAndTag &newMatch) override;

    void deleteItemAndTagMatch(const ItemDetailAndTag &match) override;

    void deleteItemAndTagMatchByItemID(const QString &itemID) override;

    void deleteItemAndTagMatchByItemIDs(const QList<QString> &itemIDList) override;

    void deleteItemAndTagMatchByTagID(const QString &tagID) override;
    // End

    // For notification
    QList<ItemDetail> selectNextNotifiedItemDetail() override;
    // End

    // ItemGroup
    QList<ItemGroupDao> selectItemGroupByID(const QString &groupID) override;

    QList<ItemGroupDao> selectItemGroupByType(const todo::ItemGroupType &type) override;

    void updateItemGroupByID(const QString &groupID, const ItemGroupDao &itemGroupDao) override;

    void deleteItemGroupByID(const QString &groupID) override;

    void deleteItemGroupByIDs(const QList<QString> &groupIDList) override;

    void insertItemGroup(const ItemGroupDao &itemGroupDao) override;

    QList<ItemGroupDao> selectItemGroupByIDs(const QList<QString> &groupIDs) override;
    // End

    // ItemGroupRelation
    QList<ItemGroupRelation> selectItemGroupRelationByRootID(const QString &rootID) override;

    void
    deleteItemGroupRelationByDirectParentIDAndItemID(const QString &directParentID, const QString &itemID) override;

    void deleteItemGroupRelationByRootID(const QString &rootID) override;

    void deleteItemGroupRelationByItemID(const QString &itemID) override;

    void deleteItemGroupRelationByItemIDs(const QList<QString> &itemIDList) override;

    void insertItemGroupRelation(const ItemGroupRelation &relation) override;

    QList<ItemGroupRelation> selectItemGroupRelationByParentID(const QString &parentID) override;

    QList<ItemGroupRelation> selectItemGroupRelationByItemID(const QString &itemID) override;

    QList<ItemGroupRelation> selectAllItemGroupRelation() override;
    // End

    // StickyNoteDao
    QList<StickyNoteDao> selectStickyNoteById(const QString &id) override;

    QList<StickyNoteDao> selectAllStickyNote() override;

    void insertStickyNote(const StickyNoteDao &stickyNoteDao) override;

    void updateStickyNoteById(const QString &id, const StickyNoteDao &stickyNoteDao) override;

    void updateStickyNotePositionById(const QString &id, int x, int y) override;

    void deleteStickyNoteById(const QString &id) override;
    // End

    // ItemDetailTimeDao
    QList<ItemDetailTimeDao> selectItemDetailTimeByItemID(const QString &itemID) override;

    QList<ItemDetailTimeDao> selectItemDetailTimeByTargetDate(const QDate &targetDate) override;

    void insertItemDetailTime(const ItemDetailTimeDao &dao) override;

    void deleteItemDetailTimeByItemID(const QString &itemID) override;

    void deleteItemDetailTimeByItemIDs(const QList<QString> &itemIDs) override;
    // End

    // for transaction
    void startTransaction() override;

    void endTransaction(bool isSuccessful) override;
    //end

    void init() override;

    ~SQLDao() override;

private:
    void createTables();
    QString generateWhereClauseStrValues(const QString &fieldName, const QString &operation, bool isAnd, const QList<QString> &valueList);
    QSqlDatabase db;
    int sqlType; // 0: MySQL, 1: SQLite
};

}

#endif //TODOMANAGER_MYSQLDAO_H
