//
// Created by septemberhx on 7/16/18.
//

#ifndef TODOMANAGER_ITEMGROUP_H
#define TODOMANAGER_ITEMGROUP_H

#include <QString>
#include <QDateTime>
#include <QList>
#include "ItemDetail.h"

namespace todo {

enum ItemGroupType {
    PROJECT,  // top project
    SUB_PROJECT  // child project. For sql operation simplify
};

/**
 * Group items together like a Project or SubProject, etc.
 */
class ItemGroup {
public:
    ItemGroup();

    const QString &getTitle() const;

    void setTitle(const QString &title);

    const QString &getId() const;

    void setId(const QString &id);

    const QString &getDescription() const;

    void setDescription(const QString &description);

    ItemGroupType getType() const;

    void setType(ItemGroupType type);

    bool isMileStone() const;

    void setMileStone(bool mileStone);

    const QDateTime &getCreatedTime() const;

    void setCreatedTime(const QDateTime &createdTime);

    const QDateTime &getLastUpdatedTime() const;

    void setLastUpdatedTime(const QDateTime &lastUpdatedTime);

    const QList<ItemGroup> &getSubGroupList() const;

    void setSubGroupList(const QList<ItemGroup> &subGroupList);

    const QList<ItemDetail> &getItemDetailList() const;

    void setItemDetailList(const QList<ItemDetail> &itemDetailList);

    const QDate &getFromDate() const;

    void setFromDate(const QDate &fromDate);

    const QDate &getToDate() const;

    void setToDate(const QDate &toDate);

    void addItemDetail(const ItemDetail &itemDetail);

    void addItemGroup(const ItemGroup &itemGroup);

    bool operator==(const ItemGroup &otherGroup) const;

private:
    // persistence in sql
    QString title;
    QString id;
    QString description;
    ItemGroupType type;
    bool mileStone;
    QDateTime createdTime;
    QDateTime lastUpdatedTime;
    QDate fromDate;
    QDate toDate;

    // store in ItemGroupRelation
    QList<ItemGroup> subGroupList;
    QList<ItemDetail> itemDetailList;
};

}

#endif //TODOMANAGER_ITEMGROUP_H
