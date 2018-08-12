//
// Created by septemberhx on 7/18/18.
//

#include <gtest/gtest.h>
#include "../utils/ItemGroupUtils.h"

TEST(ItemGroupUtils, getGroupOverview) {
    todo::ItemGroup rootGroup;
    todo::ItemDetail item1_level1("1-1");
    todo::ItemDetail item2_level1("1-2");

    rootGroup.addItemDetail(item1_level1.getId());
    rootGroup.addItemDetail(item2_level1.getId());

    auto overView1 = todo::ItemGroupUtils::getGroupOverview(rootGroup);
    EXPECT_EQ(overView1.getSubItemCount(), 2);
    EXPECT_EQ(overView1.getSubGroupCount(), 0);

    todo::ItemGroup subGroup;
    todo::ItemDetail item1_level2("2-1");
    todo::ItemDetail item2_level2("2-2");
    item2_level2.setDone(true);
    subGroup.addItemDetail(item1_level2.getId());
    subGroup.addItemDetail(item2_level2.getId());
    rootGroup.addItemGroup(subGroup.getId());

    auto overView2 = todo::ItemGroupUtils::getGroupOverview(rootGroup);
    EXPECT_EQ(overView2.getSubItemCount(), 2);
    EXPECT_EQ(overView2.getSubGroupCount(), 1);
    EXPECT_EQ(overView2.getTotalItemCount(), 4);
    EXPECT_EQ(overView2.getTotalItemDoneCount(), 1);

    todo::ItemGroup subGroup2;
    todo::ItemDetail item3_level2("2-3");
    item3_level2.setDone(true);
    subGroup2.addItemDetail(item3_level2.getId());
    rootGroup.addItemGroup(subGroup2.getId());

    auto overView3 = todo::ItemGroupUtils::getGroupOverview(rootGroup);
    EXPECT_EQ(overView3.getSubGroupDoneCount(), 1);
    EXPECT_EQ(overView3.getSubGroupCount(), 2);
    EXPECT_EQ(overView3.getTotalItemCount(), 5);
    EXPECT_EQ(overView3.getTotalItemDoneCount(), 2);
}

TEST(ItemGroupUtils, buildGroup) {
    todo::ItemDetail itemDetail1("1");
    todo::ItemDetail itemDetail2("2");
    todo::ItemGroup itemGroup1;
    itemGroup1.setType(todo::ItemGroupType::PROJECT);
    todo::ItemGroup subGroup1;
    subGroup1.setType(todo::ItemGroupType::SUB_PROJECT);

    todo::ItemGroupRelation r1;
    r1.setRootGroupID(itemGroup1.getId());
    r1.setDirectGroupID(subGroup1.getId());
    r1.setItemID(itemDetail1.getId());

    todo::ItemGroupRelation r2;
    r2.setRootGroupID(itemGroup1.getId());
    r2.setDirectGroupID(itemGroup1.getId());
    r2.setItemID(itemDetail2.getId());

    todo::ItemGroupRelation r3;
    r3.setRootGroupID(itemGroup1.getId());
    r3.setDirectGroupID(itemGroup1.getId());
    r3.setItemID(subGroup1.getId());

    QList<todo::ItemDetail> itemList{itemDetail1, itemDetail2};
    QList<todo::ItemGroupRelation> relationList{r1, r2, r3};
    QList<todo::ItemGroup> groupList{itemGroup1, subGroup1};
}
