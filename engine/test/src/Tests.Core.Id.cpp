#include "starlight/core/Id.hh"

#include <gtest/gtest.h>

using namespace sl;

static constexpr Id8::Type value1 = 1;
static constexpr Id8::Type value2 = 7;

TEST(IdTests, emptyIdToBoolean) {
    Id8 id;

    EXPECT_FALSE(id.hasValue());
}

TEST(IdTests, idToBoolean) {
    Id8 id(value1);
    EXPECT_TRUE(id.hasValue());
}

TEST(IdTests, getValue) {
    Id8 id(value1);
    ASSERT_TRUE(id.hasValue());
    EXPECT_EQ(value1, *id);
    EXPECT_EQ(value1, id.get());
}

TEST(IdTests, reassignValue) {
    Id8 id(value1);
    EXPECT_EQ(value1, *id);
    id = value2;
    EXPECT_EQ(value2, *id);
}

TEST(IdTests, invalidate) {
    Id8 id(value1);
    ASSERT_TRUE(id.hasValue());
    id.invalidate();
    EXPECT_FALSE(id.hasValue());
}
