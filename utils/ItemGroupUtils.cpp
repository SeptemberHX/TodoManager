//
// Created by septemberhx on 7/17/18.
//

#include "ItemGroupUtils.h"
#include <QMap>
#include <QList>
#include <QDebug>

todo::DataCenter todo::ItemGroupUtils::dataCenter;

todo::ItemGroupOverview todo::ItemGroupUtils::getGroupOverview(const todo::ItemGroup &itemGroup) {
    ItemGroupOverview overview;

    int totalItemCount = 0;
    int totalItemDoneCount = 0;
    int subGroupDoneCount = 0;
    auto subGroupList = ItemGroupUtils::dataCenter.selectItemGroupByIDs(itemGroup.getSubGroupIDList());
    foreach (auto const &childGroup, subGroupList) {
        auto childOverview = ItemGroupUtils::getGroupOverview(childGroup);
        if (childOverview.isDone()) {
            ++subGroupDoneCount;
        }

        totalItemCount += childOverview.getTotalItemCount();
        totalItemDoneCount += childOverview.getTotalItemDoneCount();
    }

    int subItemDoneCount = 0;
    auto subItemList = ItemGroupUtils::dataCenter.selectItemDetailByIDs(itemGroup.getItemDetailIDList());
    foreach (auto const &item, subItemList) {
        if (item.isDone()) {
            ++subItemDoneCount;
        }
    }
    totalItemCount += itemGroup.getItemDetailIDList().size();
    totalItemDoneCount += subItemDoneCount;

    overview.setSubGroupCount(itemGroup.getSubGroupIDList().size());
    overview.setSubGroupDoneCount(subGroupDoneCount);
    overview.setSubItemCount(itemGroup.getItemDetailIDList().size());
    overview.setSubItemDoneCount(subItemDoneCount);
    overview.setTotalItemCount(totalItemCount);
    overview.setTotalItemDoneCount(totalItemDoneCount);

    return overview;
}

QList<QString> todo::ItemGroupUtils::getGroupPath(const QString &subGroupID) {
    auto relations = ItemGroupUtils::dataCenter.selectItemGroupRelationByItemID(subGroupID);
    qDebug() << relations.size();
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

    qDebug() << resultPathList.size();
    return resultPathList;
}

QMap<QString, QList<QString>> todo::ItemGroupUtils::getFullRelationTree() {
    QMap<QString, QList<QString>> fullRelationTreeMap;

    auto relations = ItemGroupUtils::dataCenter.selectAllItemGroupRelation();
    foreach (auto const &relation, relations) {
        if (!fullRelationTreeMap.contains(relation.getDirectGroupID())) {
            fullRelationTreeMap.insert(relation.getDirectGroupID(), QList<QString>());
        }

        fullRelationTreeMap[relation.getDirectGroupID()].append(relation.getItemID());
    }

    return fullRelationTreeMap;
}
