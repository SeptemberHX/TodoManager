//
// Created by septemberhx on 7/27/18.
//

#include "ItemUtils.h"
#include <QUuid>

todo::DataCenter todo::ItemUtils::dataCenter;

todo::ItemDetail
todo::ItemUtils::generateNewItemDetail(const QString &title, const QString &rootItemID, const QString &directItemID) {
    todo::ItemDetail newItemDetail(title);
    newItemDetail.setRootGroupID(rootItemID);
    newItemDetail.setDirectGroupID(directItemID);
    newItemDetail.setCreatedTime(QDateTime::currentDateTime());
    newItemDetail.setLastUpdatedTime(QDateTime::currentDateTime());
    newItemDetail.setDone(false);
    newItemDetail.setMode(ItemMode::SIMPLE);
    return newItemDetail;
}

QColor todo::ItemUtils::getRootGroupColor(const todo::ItemAndGroupWrapper &wrapper) {
    if (wrapper.hasRootGroup()) {
        return ItemUtils::dataCenter.selectItemGroupByID(wrapper.getRootGroupID())[0].getColor();
    } else {
        return Qt::transparent;
    }
}

void todo::ItemUtils::assignItemDetailToGroup(const QString &itemID, const QString &rootGroupID, const QString &directGroupID) {
    ItemUtils::assignItemDetailsToGroup({itemID}, rootGroupID, directGroupID);
}

void todo::ItemUtils::assignItemDetailsToGroup(const QList<QString> &itemIDs, const QString &rootGroupID, const QString &directGroupID) {
    foreach (auto const &itemID, itemIDs) {
        ItemUtils::dataCenter.deleteItemGroupRelationByItemID(itemID);
        ItemGroupRelation relation;
        relation.setRootGroupID(rootGroupID);
        relation.setDirectGroupID(directGroupID);
        relation.setItemID(itemID);
        ItemUtils::dataCenter.insertItemGroupRelation(relation);
    }
}

todo::ItemDetail todo::ItemUtils::assignItemDetailToGroup(const todo::ItemDetail &itemDetail, const QString &rGroupID,
                                                          const QString &dGroupID) {
    return ItemUtils::assignItemDetailsToGroup({itemDetail}, rGroupID, dGroupID)[0];
}

QList<todo::ItemDetail>
todo::ItemUtils::assignItemDetailsToGroup(const QList<todo::ItemDetail> &itemDetails, const QString &rGroupID,
                                          const QString &dGroupID) {
    QList<ItemDetail> resultList;
    QList<QString> itemIDs;
    foreach (auto const &item, itemDetails) {
        itemIDs.append(item.getId());
        resultList.append(item);
        resultList.last().setRootGroupID(rGroupID);
        resultList.last().setDirectGroupID(dGroupID);
    }
    ItemUtils::assignItemDetailsToGroup(itemIDs, rGroupID, dGroupID);
    return resultList;
}
