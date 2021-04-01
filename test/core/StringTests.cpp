#include "sl/core/String.hpp"

#include <gtest/gtest.h>

using namespace sl::core;

namespace {

TEST(StringTests, whenFormattingString_shouldGiveCorrectAnswer) {
    ASSERT_EQ("Hello, 1, 3, 6.0, world", format("Hello, %d, %d, %.1f, %s", 1, 3, 6.0f, "world"));
}

TEST(StringTests, givenEmptyString_whenCheckingIfStringEmpty_shouldReturnTrue) {
    auto emptyString = "";
    ASSERT_TRUE(isStringEmpty(emptyString));
}

TEST(StringTests, givenNonEmptyString_whenCheckingIfStringEmpty_shouldReturnFalse) {
    auto nonEmptyString = "i'm not empty";
    ASSERT_FALSE(isStringEmpty(nonEmptyString));
}
}
