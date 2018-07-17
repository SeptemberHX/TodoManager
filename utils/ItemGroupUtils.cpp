//
// Created by septemberhx on 7/17/18.
//

#include "ItemGroupUtils.h"
#include <QMap>
#include <QList>
#include <QDebug>

QList<todo::ItemGroup>
todo::ItemGroupUtils::buildGroup(const QList<todo::ItemGroup> &itemGroupList, const QList<todo::ItemDetail> &itemList,
                                 const QList<todo::ItemGroupRelation> &relationList) {
    QMap<QString, ItemGroup> itemGroupMap;
    QMap<QString, ItemDetail> itemMap;
    QMap<QString, QList<ItemGroupRelation>> relationListMap;
    QList<ItemGroup> rootGroupList;

    // prepare for calculation
    foreach(const ItemGroup &itemGroup, itemGroupList) {
        if (itemGroup.getType() == ItemGroupType::PROJECT) {
            rootGroupList.append(itemGroup);
        } else {
            qDebug() << typeid(itemGroup).name();
            itemGroupMap.insert(itemGroup.getId(), itemGroup);
        }
    }
    foreach(auto const &item, itemList) {
        itemMap.insert(item.getId(), item);
    }
    foreach(auto const &relation, relationList) {
        if (!relationListMap.contains(relation.getDirectGroupID())) {
            relationListMap.insert(relation.getDirectGroupID(), QList<ItemGroupRelation>());
        }
        relationListMap[relation.getDirectGroupID()].append(relation);
    }

    // calculate
    QList<ItemGroup> resultList;
    foreach(auto const &rootGroup, rootGroupList) {
        ItemGroup currRootGroup = rootGroup;
        buildGroup_(&currRootGroup, itemGroupMap, itemMap, relationListMap);
        resultList.append(currRootGroup);
    }

    return resultList;
}

void
todo::ItemGroupUtils::buildGroup_(todo::ItemGroup *rootGroupPtr, const QMap<QString, todo::ItemGroup> &groupMap,
                                  const QMap<QString, todo::ItemDetail> &detailMap,
                                  const QMap<QString, QList<todo::ItemGroupRelation>> &relationListMap) {
    if (relationListMap.contains(rootGroupPtr->getId())) {
        foreach (auto &relation, relationListMap[rootGroupPtr->getId()]) {
            if (detailMap.contains(relation.getItemID())) {
                rootGroupPtr->addItemDetail(detailMap[relation.getItemID()]);
            } else {
                ItemGroup childGroup = groupMap[relation.getItemID()];
                buildGroup_(&childGroup, groupMap, detailMap, relationListMap);
                rootGroupPtr->addItemGroup(childGroup);
            }
        }
    }
}
