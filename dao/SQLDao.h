//
// Created by septemberhx on 5/17/18.
//

#ifndef TODOMANAGER_MYSQLDAO_H
#define TODOMANAGER_MYSQLDAO_H

#include <QSqlDatabase>
#include "AbstractDao.h"
#include "../core/ItemDetail.h"

namespace todo {

class SQLDao : public AbstractDao {
public:
    SQLDao();

    SQLDao(int type);

    // ItemDetail
    QList<ItemDetail> selectItemDetailByDate(const QDate &targetDate) override;

    void updateItemDetailByID(const QString &itemID, const ItemDetail &itemDetail) override;

    void deleteItemDetailByID(const QString &itemID) override;

    void insertItemDetail(const ItemDetail &itemDetail) override;

    void updateDoneByID(const QString &itemID, bool flag) override;

    QList<ItemDetail> selectItemDetailByDate(const QDate &fromDate, const QDate &toDate) override;

    QList<ItemDetail> selectItemDetailByIDs(const QList<QString> &itemIDs) override;
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
    // End

    // For notification
    QList<ItemDetail> selectNextNotifiedItemDetail() override;
    // End

    // ItemGroup
    QList<ItemGroup> selectItemGroupByID(const QString &groupID) override;

    void updateItemGroupByID(const QString &groupID, const ItemGroup &itemGroup) override;

    void deleteItemGroupByID(const QString &groupID) override;

    void insertItemGroup(const ItemGroup &itemGroup) override;

    QList<ItemGroup> selectItemGroupByIDs(const QList<QString> &groupIDs) override;
    // End

    void init() override;

    ~SQLDao() override;

private:
    void createTables();
    QSqlDatabase db;
    int sqlType; // 0: MySQL, 1: SQLite
};

}

#endif //TODOMANAGER_MYSQLDAO_H
