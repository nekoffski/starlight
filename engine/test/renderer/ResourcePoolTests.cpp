
#include <gtest/gtest.h>

#include "starlight/core/utils/ResourcePool.hpp"

#include "mock/StubResource.h"

using namespace sl;
using namespace testing;

struct ResourcePoolTests : Test {
    inline static const std::string name{ "Resource" };
    static constexpr u64 size = 8;
    static constexpr int x    = 1;
    static constexpr float y  = 2.5f;

    ResourcePool<StubResource> pool{ name, size };
};

TEST_F(
  ResourcePoolTests, givenResourcePool_whenGettingSize_shouldReturnCorrectSize
) {
    ASSERT_EQ(pool.getSize(), size);
}

TEST_F(
  ResourcePoolTests, givenResourcePool_whenGettingName_shouldReturnCorrectName
) {
    ASSERT_EQ(pool.getResourceName(), name);
}

TEST_F(ResourcePoolTests, givenResourcePool_whenAddingItem_shouldReturnPointer) {
    auto item = pool.create(x, y);
    ASSERT_TRUE(item);
    EXPECT_EQ(item->x, x);
    EXPECT_EQ(item->y, y);
    EXPECT_EQ(item->id, 0);
}

TEST_F(
  ResourcePoolTests, givenResourcePool_whenGettingAdding_shouldReturnCorrectObject
) {
    auto itemPointer = pool.create(x, y);
    auto& item       = pool.get(itemPointer->id);

    EXPECT_EQ(itemPointer, &item);
}

TEST_F(ResourcePoolTests, givenResourcePool_whenDestroyingItem_shouldDestroy) {
    auto itemPointer = pool.create(x, y);
    ASSERT_NO_THROW(pool.destroy(itemPointer->id));
}

TEST_F(
  ResourcePoolTests,
  givenResourcePool_whenCreatingItemWhenPoolIsOutOfCapacity_shouldReturnNullptr
) {
    for (int i = 0; i < pool.getSize(); ++i) {
        auto item = pool.create(x, y);
        EXPECT_EQ(item->id, i);
    }
    EXPECT_EQ(pool.create(x, y), nullptr);
}