//
// Created by septemberhx on 5/10/18.
//

#include <QDebug>
#include "DataCenter.h"

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

    emit(this->itemDetailModified());
}

void todo::DataCenter::deleteItemDetailByID(const QString &itemID) {
    DaoFactory::getInstance()->getSQLDao()->deleteItemDetailByID(itemID);
    DaoFactory::getInstance()->getSQLDao()->deleteItemAndTagMatchByItemID(itemID);

    emit(this->itemDetailModified());
}

void todo::DataCenter::insertItemDetail(const todo::ItemDetail &itemDetail) {
    DaoFactory::getInstance()->getSQLDao()->insertItemDetail(itemDetail);
    int i = 1;
    for (auto const &tag : itemDetail.getTags()) {
        DaoFactory::getInstance()->getSQLDao()->insertItemAndTagMatch(ItemDetailAndTag(itemDetail.getId(), tag.getId(), i));
        ++i;
    }

    emit(this->itemDetailModified());
}

void todo::DataCenter::updateDoneByID(const QString &itemID, bool flag) {
    DaoFactory::getInstance()->getSQLDao()->updateDoneByID(itemID, flag);

    emit(this->itemDetailModified());
}

QList<todo::ItemTag> todo::DataCenter::selectItemTagById(const QString &tagId) {
    return DaoFactory::getInstance()->getSQLDao()->selectItemTagById(tagId);
}

void todo::DataCenter::updateItemTagById(const QString &tagId, const todo::ItemTag &itemTag) {
    DaoFactory::getInstance()->getSQLDao()->updateItemTagById(tagId, itemTag);
}

void todo::DataCenter::deleteItemTagById(const QString &tagId) {
    DaoFactory::getInstance()->getSQLDao()->deleteItemTagById(tagId);
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
