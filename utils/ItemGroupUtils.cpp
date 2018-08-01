//
// Created by septemberhx on 7/17/18.
//

#include "ItemGroupUtils.h"
#include <QMap>
#include <QList>
#include <QDebug>

todo::DataCenter todo::ItemGroupUtils::dataCenter;

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

todo::ItemGroupOverview todo::ItemGroupUtils::getGroupOverview(const todo::ItemGroup &itemGroup) {
    ItemGroupOverview overview;

    int totalItemCount = 0;
    int totalItemDoneCount = 0;
    int subGroupDoneCount = 0;
    foreach (auto const &childGroup, itemGroup.getSubGroupList()) {
        auto childOverview = ItemGroupUtils::getGroupOverview(childGroup);
        if (childOverview.isDone()) {
            ++subGroupDoneCount;
        }

        totalItemCount += childOverview.getTotalItemCount();
        totalItemDoneCount += childOverview.getTotalItemDoneCount();
    }

    int subItemDoneCount = 0;
    foreach (auto const &item, itemGroup.getItemDetailList()) {
        if (item.isDone()) {
            ++subItemDoneCount;
        }
    }
    totalItemCount += itemGroup.getItemDetailList().size();
    totalItemDoneCount += subItemDoneCount;

    overview.setSubGroupCount(itemGroup.getSubGroupList().size());
    overview.setSubGroupDoneCount(subGroupDoneCount);
    overview.setSubItemCount(itemGroup.getItemDetailList().size());
    overview.setSubItemDoneCount(subItemDoneCount);
    overview.setTotalItemCount(totalItemCount);
    overview.setTotalItemDoneCount(totalItemDoneCount);

    return overview;
}

QList<QString> todo::ItemGroupUtils::getGroupPath(const QString &subGroupID) {
    auto relations = ItemGroupUtils::dataCenter.selectItemGroupRelationByItemID(subGroupID);
    if (relations.empty()) {
        return {subGroupID};
    }

    auto fullRelations = ItemGroupUtils::dataCenter.selectItemGroupRelationByRootID(relations[0].getRootGroupID());
    QMap<QString, ItemGroupRelation> id2RelationMap;
    foreach (auto const &relation, fullRelations) {
        id2RelationMap.insert(relation.getItemID(), relation);  // only work when item belongs to ONE GROUP
    }
    QList<QString> resultPathList;
    resultPathList.append(subGroupID);
    while (id2RelationMap.contains(resultPathList.first())) {
        resultPathList.push_front(id2RelationMap[resultPathList.first()].getDirectGroupID());
    }

    return resultPathList;
}
