//
// Created by septemberhx on 4/24/18.
//

#include <gtest/gtest.h>
#include "../core/ItemTag.h"

TEST(core, ItemTag) {
    todo::ItemTag tag1("1");
    todo::ItemTag tag2("1");
    todo::ItemTag tag3("2");
    EXPECT_EQ(tag1, tag2);
    EXPECT_NE(tag1, tag3);
}
