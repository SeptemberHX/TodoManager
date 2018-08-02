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

    const QList<QString> &getSubGroupIDList() const;

    void setSubGroupIDList(const QList<QString> &subGroupIDList);

    const QList<QString> &getItemDetailIDList() const;

    void setItemDetailIDList(const QList<QString> &itemDetailIDList);

    const QDate &getFromDate() const;

    void setFromDate(const QDate &fromDate);

    const QDate &getToDate() const;

    void setToDate(const QDate &toDate);

    const QColor &getColor() const;

    void setColor(const QColor &color);

    void addItemDetail(const QString &itemDetailID);

    void addItemGroup(const QString &itemGroupID);

    bool operator==(const ItemGroup &otherGroup) const;

    bool hasRootGroup() const;

    const QString &getDirectGroupID() const;

    void setDirectGroupID(const QString &directGroupID);

    const QString &getRootGroupID() const;

    void setRootGroupID(const QString &rootGroupID);

private:
    ItemGroupDao itemGroupDao;
    QString directGroupID;
    QString rootGroupID;
    // only one level
    QList<QString> subGroupIDList;
    QList<QString> itemDetailIDList;
};

}

#endif //TODOMANAGER_ITEMGROUP_H
