//
// Created by septemberhx on 5/10/18.
//

#ifndef TODOMANAGER_DATACENTER_H
#define TODOMANAGER_DATACENTER_H


#include <QString>
#include <QList>
#include "../core/ItemDetail.h"
#include "../dao/DaoFactory.h"
#include "../core/ItemDetailAndTag.h"

namespace todo {

/**
 * DataCenter handles all data operations.
 * Other modules can only get data from DataCenter regardless of it's implementation.
 * DataCenter will use dao/DaoFactory to do next job.
 */
class DataCenter {
public:

    // ---------------- todo_item.item_detail Starts ----------------
    QList<ItemDetail> selectItemDetailByDate(const QDate &targetDate);
    void updateItemDetailByID(const QString &itemID, const ItemDetail &itemDetail);
    void deleteItemDetailByID(const QString &itemID);
    void insertItemDetail(const ItemDetail &itemDetail);
    void updateDoneByID(const QString &itemID, bool flag);
    // ---------------- todo_item.item_detail Ends ----------------

    // ---------------- todo_item.tags Starts -----------------
    QList<ItemTag> selectAllItemTag();
    QList<ItemTag> selectItemTagById(const QString &tagId);
    void updateItemTagById(const QString &tagId, const ItemTag &itemTag);
    void deleteItemTagById(const QString &tagId);
    void insertItemTag(const ItemTag &tag);
    // ---------------- todo_item.tags Ends -----------------

private:
    void fillTagInfo(QList<ItemDetail> &itemDetails);
};

}

#endif //TODOMANAGER_DATACENTER_H
