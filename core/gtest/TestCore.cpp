//
// Created by septemberhx on 4/24/18.
//

#include <gtest/gtest.h>
#include "../ItemTag.h"

TEST(core_test_case, core_test) {
    todo::ItemTag tag1("1");
    todo::ItemTag tag2("1");
    todo::ItemTag tag3("2");
    EXPECT_EQ(tag1, tag2);
    EXPECT_NE(tag1, tag3);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}