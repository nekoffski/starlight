#include "starlight/core/containers/StaticVector.hh"

#include <gtest/gtest.h>

using namespace sl;

constexpr u64 defaultCapacity = 10;

TEST(StaticVectorTests, empty) {
    EXPECT_EQ(StaticVector<int>{ defaultCapacity }.getSize(), 0);
}

TEST(StaticVectorTests, capacity) {
    EXPECT_EQ(StaticVector<int>{ defaultCapacity }.getCapacity(), defaultCapacity);
}

TEST(StaticVectorTests, insertValue) {
    StaticVector<int> v{ defaultCapacity };
    auto value = v.emplace(1337);
    ASSERT_TRUE(value != nullptr);
    EXPECT_EQ(*value, 1337);
    EXPECT_EQ(v.getSize(), 1);
}

TEST(StaticVectorTests, eraseValue) {
    StaticVector<int> v{ defaultCapacity };
    auto value = v.emplace(1337);
    ASSERT_TRUE(value != nullptr);
    ASSERT_EQ(v.getSize(), 1);

    EXPECT_TRUE(v.erase(value));
    EXPECT_EQ(v.getSize(), 0);
}

TEST(StaticVectorTests, eraseNonExisting) {
    int v = 1337;
    EXPECT_FALSE(StaticVector<int>{ defaultCapacity }.erase(&v));
}

TEST(StaticVectorTests, insertOverCapacity) {
    StaticVector<int> v{ 1 };
    v.emplace(1);
    EXPECT_EQ(v.emplace(1337), nullptr);
}

TEST(StaticVectorTests, emptyNoCapacity) { EXPECT_DEATH(StaticVector<int>{}, ""); }
