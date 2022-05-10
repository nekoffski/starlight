#include "nova/core/Memory.hpp"

#include <gtest/gtest.h>

using namespace nova;

constexpr int value = 1337;

TEST(MemoryTests, whenCreatingUniqPtr_shouldReturnCorrectPointer) {
    auto ptr = core::createUniqPtr<int>(value);
    ASSERT_NE(ptr.get(), nullptr);
    ASSERT_EQ(*ptr, value);
}

TEST(MemoryTests, whenCreatingRefPtr_shouldReturnCorrectPointer) {
    auto ptr = core::createRefPtr<int>(value);
    ASSERT_NE(ptr.get(), nullptr);
    ASSERT_EQ(*ptr, value);
}
