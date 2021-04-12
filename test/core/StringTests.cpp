#include "sl/core/String.hpp"

#include <gtest/gtest.h>

using namespace sl::core;

namespace {

TEST(StringTests, givenEmptyString_whenCheckingIfStringEmpty_shouldReturnTrue) {
    auto emptyString = "";
    ASSERT_TRUE(isStringEmpty(emptyString));
}

TEST(StringTests, givenNonEmptyString_whenCheckingIfStringEmpty_shouldReturnFalse) {
    auto nonEmptyString = "i'm not empty";
    ASSERT_FALSE(isStringEmpty(nonEmptyString));
}
}
