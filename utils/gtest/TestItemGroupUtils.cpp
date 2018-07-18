//
// Created by septemberhx on 7/18/18.
//

#include <gtest/gtest.h>
#include "../ItemGroupUtils.h"

TEST(ItemGroupUtils, getGroupOverview) {
    todo::ItemGroup rootGroup;
    todo::ItemDetail item1_level1("1-1");
    todo::ItemDetail item2_level1("1-2");

    rootGroup.addItemDetail(item1_level1);
    rootGroup.addItemDetail(item2_level1);

    auto overView1 = todo::ItemGroupUtils::getGroupOverview(rootGroup);
    EXPECT_EQ(overView1.getSubItemCount(), 2);
    EXPECT_EQ(overView1.getSubGroupCount(), 0);

    todo::ItemGroup subGroup;
    todo::ItemDetail item1_level2("2-1");
    todo::ItemDetail item2_level2("2-2");
    item2_level2.setDone(true);
    subGroup.addItemDetail(item1_level2);
    subGroup.addItemDetail(item2_level2);
    rootGroup.addItemGroup(subGroup);

    auto overView2 = todo::ItemGroupUtils::getGroupOverview(rootGroup);
    EXPECT_EQ(overView2.getSubItemCount(), 2);
    EXPECT_EQ(overView2.getSubGroupCount(), 1);
    EXPECT_EQ(overView2.getTotalItemCount(), 4);
    EXPECT_EQ(overView2.getTotalItemDoneCount(), 1);

    todo::ItemGroup subGroup2;
    todo::ItemDetail item3_level2("2-3");
    item3_level2.setDone(true);
    subGroup2.addItemDetail(item3_level2);
    rootGroup.addItemGroup(subGroup2);

    auto overView3 = todo::ItemGroupUtils::getGroupOverview(rootGroup);
    EXPECT_EQ(overView3.getSubGroupDoneCount(), 1);
    EXPECT_EQ(overView3.getSubGroupCount(), 2);
    EXPECT_EQ(overView3.getTotalItemCount(), 5);
    EXPECT_EQ(overView3.getTotalItemDoneCount(), 2);
}

TEST(ItemGroupUtils, buildGroup) {
    EXPECT_EQ(1, 1);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}