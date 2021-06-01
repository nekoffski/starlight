#include <gtest/gtest.h>

#include "sl/core/OptionalPtr.hpp"

using namespace sl::core;

TEST(OptionalPtrTests, givenEmptyPtr_whenCheckingIfHasValue_shouldReturnFalse) {
    OptionalPtr<int> ptr;

    EXPECT_FALSE(ptr.hasValue());
    EXPECT_FALSE(ptr);
}

TEST(OptionalPtrTests, givenPtrBuildFromSharedPtr_shouldStoreValue) {
    std::shared_ptr<int> sptr = std::make_shared<int>(5);

    OptionalPtr<int> ptr = sptr;

    EXPECT_TRUE(ptr.hasValue());
    EXPECT_TRUE(ptr);

    EXPECT_EQ(*ptr, *sptr);
    EXPECT_EQ(ptr.value(), *sptr);
}

TEST(OptionalPtrTests, givenPtrBuildFromUniquePtr_shouldStoreValue) {
    std::unique_ptr<int> sptr = std::make_unique<int>(5);

    OptionalPtr<int> ptr = sptr;

    EXPECT_TRUE(ptr.hasValue());
    EXPECT_TRUE(ptr);

    EXPECT_EQ(*ptr, *sptr);
    EXPECT_EQ(ptr.value(), *sptr);
}

TEST(OptionalPtrTests, givenPtr_shouldStoreValue) {
    int x = 5;
    OptionalPtr<int> ptr = &x;

    EXPECT_TRUE(ptr.hasValue());
    EXPECT_TRUE(ptr);

    EXPECT_EQ(x, *ptr);
    EXPECT_EQ(x, ptr.value());
}

TEST(OptionalPtrTests, givenPtrForConst_shouldStoreValue) {
    const int x = 5;
    OptionalPtr<const int> ptr = &x;

    EXPECT_TRUE(ptr.hasValue());
    EXPECT_TRUE(ptr);

    EXPECT_EQ(x, *ptr);
    EXPECT_EQ(x, ptr.value());
}

TEST(OptionalPtrTests, givenPtr_whenCallingReset_shouldClearPtr) {
    int x = 5;
    OptionalPtr<int> ptr = &x;

    ASSERT_TRUE(ptr.hasValue());
    ASSERT_TRUE(ptr);

    ptr.reset();

    EXPECT_FALSE(ptr.hasValue());
    EXPECT_FALSE(ptr);
}

TEST(OptionalPtrTests, givenPtr_whenChangingOriginalValue_shouldChangeStoredValue) {
    int x = 5;
    OptionalPtr<int> ptr = &x;

    ASSERT_TRUE(ptr.hasValue());
    ASSERT_TRUE(ptr);

    EXPECT_EQ(ptr.value(), x);

    x++;

    EXPECT_EQ(ptr.value(), x);
}

TEST(OptionalPtrTests, givenPtrToStruct_whenUsingArrowOperator_shouldAllowAccessingFields) {
    struct SomeStruct {
        int x;

        bool foo() {
            return true;
        }

    } someStruct { 5 };

    OptionalPtr<SomeStruct> ptr = &someStruct;

    ASSERT_TRUE(ptr.hasValue());
    ASSERT_TRUE(ptr);

    EXPECT_EQ(someStruct.x, ptr->x);
    EXPECT_EQ(someStruct.foo(), ptr->foo());
}