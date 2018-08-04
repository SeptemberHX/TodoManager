//
// Created by septemberhx on 7/20/18.
//

#include <gtest/gtest.h>
#include "../data/DataCenter.h"

TEST(DataCenter, ItemGroup) {
    todo::DataCenter dataCenter;

    todo::ItemGroup group1;
    group1.setTitle("test_group1");
    group1.setDescription("test_group1");
    group1.setType(todo::ItemGroupType::PROJECT);

    dataCenter.insertItemGroup(group1);
    EXPECT_EQ(group1, dataCenter.selectItemGroupByID(group1.getId())[0]);

    group1.setTitle("test_group1_modified");
    dataCenter.updateItemGroupByID(group1.getId(), group1);
    EXPECT_EQ(group1, dataCenter.selectItemGroupByID(group1.getId())[0]);

    QList<QString> groupIDList{group1.getId()};
    EXPECT_EQ(group1, dataCenter.selectItemGroupByIDs(groupIDList)[0]);

    dataCenter.deleteItemGroupByID(group1.getId());
    EXPECT_EQ(0, dataCenter.selectItemGroupByID(group1.getId()).size());
}

TEST(DataCenter, ItemGroupRelation) {
    todo::DataCenter dataCenter;

    todo::ItemGroupRelation relation;
    todo::ItemGroup rootGroup;
    todo::ItemGroup subGroup;
    todo::ItemDetail itemDetail;
    relation.setRootGroupID(rootGroup.getId());
    relation.setDirectGroupID(subGroup.getId());
    relation.setItemID(itemDetail.getId());

    dataCenter.insertItemGroupRelation(relation);
    EXPECT_EQ(relation, dataCenter.selectItemGroupRelationByRootID(relation.getRootGroupID())[0]);
}