//
// Created by septemberhx on 7/16/18.
//

#ifndef TODOMANAGER_ITEMGROUP_H
#define TODOMANAGER_ITEMGROUP_H

#include "ItemGroupDao.h"
#include "ItemDetail.h"

namespace todo {

/**
 * Group items together like a Project or SubProject, etc.
 */
class ItemGroup {
public:
    ItemGroup();
    explicit ItemGroup(const ItemGroupDao &itemGroupDao);
    ItemGroupDao toDao() const;

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

    const QColor &getColor() const;

    void setColor(const QColor &color);

    void addItemDetail(const ItemDetail &itemDetail);

    void addItemGroup(const ItemGroup &itemGroup);

    bool operator==(const ItemGroup &otherGroup) const;

    const ItemGroupDao &getDirectGroup() const;

    void setDirectGroup(const ItemGroupDao &directGroup);

    const ItemGroupDao &getRootGroup() const;

    void setRootGroup(const ItemGroupDao &rootGroup);

    bool hasRootGroup() const;

private:
    ItemGroupDao itemGroupDao;
    ItemGroupDao directGroup;
    ItemGroupDao rootGroup;
    // only one level
    QList<ItemGroup> subGroupList;
    QList<ItemDetail> itemDetailList;
};

}

#endif //TODOMANAGER_ITEMGROUP_H
