//
// Created by septemberhx on 5/10/18.
//

#ifndef TODOMANAGER_DATACENTER_H
#define TODOMANAGER_DATACENTER_H


#include <QString>
#include <QList>
#include <QObject>
#include "GlobalCache.h"
#include "../core/ItemDetailDao.h"
#include "../core/ItemDetail.h"
#include "../dao/DaoFactory.h"
#include "../core/ItemDetailAndTag.h"
#include "../core/ItemAndGroupWrapper.h"
#include "../core/StickyNote.h"

namespace todo {

/**
 * DataCenter handles all data operations.
 * Other modules can only get data from DataCenter regardless of it's implementation.
 * DataCenter will use dao/DaoFactory to do next job.
 *
 * DO NOT call function which will emit databaseModified signal in a function which will emit too.
 * Just avoid using DataCenter::func() in DataCenter::func(). Please call DaoFactory instead.
 */
class DataCenter : public QObject {
    Q_OBJECT
public:
    DataCenter(QObject *parent = 0);

    // ---------------- todo_item.item_detail Starts ----------------
    QList<ItemDetail> selectItemDetailByDate(const QDate &targetDate);
    void updateItemDetailByID(const QString &itemID, const ItemDetail &oldItemDetail, const ItemDetail &newItemDetail);
    void updateItemDetailsByIDList(const QList<QString> &itemIDList, const QList<ItemDetail> &oldItemDetailList, const QList<ItemDetail> &newItemDetailList);
    void deleteItemDetailByIDCompletely(const QString &itemID);
    void insertItemDetail(const ItemDetail &itemDetail);
    void updateDoneByID(const QString &itemID, bool flag);
    QList<ItemDetail> selectItemDetailByDate(const QDate &fromDate, const QDate &toDate);
    QList<ItemDetail> selectItemDetailByIDs(const QList<QString> &itemIDs);
    ItemDetail selectItemDetailByID(const QString &itemID);
    // ---------------- todo_item.item_detail Ends ----------------

    // ---------------- todo_item.tags Starts -----------------
    QList<ItemTag> selectAllItemTag();
    QList<ItemTag> selectItemTagById(const QString &tagId);
    void updateItemTagById(const QString &tagId, const ItemTag &itemTag);

    /**
     * delete target tag only. All items belong to it will be kept.
     * @param tagId
     */
    void deleteItemTagOnlyById(const QString &tagId);

    /**
     * delete target tag and all items belong to it
     * @param tagId
     */
    void deleteItemTagAndAllItemsByTagId(const QString &tagId);
    void insertItemTag(const ItemTag &tag);
    // ---------------- todo_item.tags Ends -----------------

    // ---------------- for notification ----------------
    QList<ItemDetail> selectNextNotifiedItemDetail();
    // ---------------- ends ------------------

    // ---------------- for TagMode ----------------
    QList<ItemDetail> selectItemDetailsByTag(const ItemTag &itemTag);
    // ---------------- ends -----------------------

    // ---------------- ItemGroup ----------------
    QList<ItemGroup> selectItemGroupByID(const QString &groupID);

    QList<ItemGroup> selectItemGroupByType(const todo::ItemGroupType &type);

    void updateItemGroupByID(const QString &groupID, const ItemGroupDao &itemGroupDao);

    void insertItemGroup(const ItemGroup &itemGroup);

    QList<ItemGroup> selectItemGroupByIDs(const QList<QString> &groupIDs);
    // ---------------- ends ---------------------

    // ItemGroupRelation
    QList<ItemGroupRelation> selectItemGroupRelationByRootID(const QString &rootID);

    QList<ItemGroupRelation> selectItemGroupRelationByItemID(const QString &itemID);

    QList<ItemGroupRelation> selectAllItemGroupRelation();
    // End

    // StickyNote
    QList<StickyNote> selectStickyNoteByID(const QString &stickyNoteId);

    QList<StickyNote> selectAllStickyNote();

    void updateStickyNotePositionById(const QString &id, int x, int y);

    void updateStickyNote(const StickyNote &stickyNote);

    void insertStickyNote(const StickyNote &stickyNote);

    void deleteStickyNoteById(const QString &id);
    // End

    // ItemDetailTimeDao
    QList<todo::ItemDetailTimeDao> selectItemDetailTimeByItemID(const QString &itemID);
    void insertItemDetailTime(const ItemDetailTimeDao &dao);


    QList<todo::ItemAndGroupWrapper> selectItemByDirectGroupID(const QString &groupID);

    // delete group completely, including all sub group and items belongs to it
    void deleteGroupCompletely(const QString &groupID);

signals:
    void databaseModified();

private:
    QList<ItemDetail> fillTagInfo(const QList<ItemDetailDao> &itemDetailDaos);
    QList<ItemDetail> fillItemDetailInfo(const QList<ItemDetailDao> &itemDetailDaos);
    QList<ItemGroup> fillItemGroupInfo(const QList<ItemGroupDao> &itemGroupDaos);

    QList<StickyNote> fillStickyNoteInfo(const QList<StickyNoteDao> &stickyNoteDaos);

    void updateItemDetailByID_(const QString &itemID, const ItemDetail &oldItemDetail, const ItemDetail &newItemDetail);

    // for recursive or avoiding emit signal repeatly
    void deleteGroupCompletely_(const QString &groupID);
    void deleteItemDetailByIDCompletely_(const QString &itemID);
    void deleteItemDetailByIDsCompletely_(const QList<QString> &itemIDList);
};

}

#endif //TODOMANAGER_DATACENTER_H
