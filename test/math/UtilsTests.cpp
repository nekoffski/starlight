#include <gtest/gtest.h>

#include "sl/math/Utils.hpp"

using namespace sl::math;
using namespace testing;

namespace {

const int low  = 1;
const int high = 15;

TEST(UtilsTests, givenValueInRange_whenCalculatingCircularRange_shouldReturnSameValue) {
    const int value = 5;

    EXPECT_EQ(circularRange(value, low, high), value);
    EXPECT_EQ(circularRange(low, low, high), low);
    EXPECT_EQ(circularRange(high, low, high), high);
}

TEST(UtilsTests, givenTooHighValue_whenCalculatingCircularRange_shouldClamp) {
    const int value = 22;
    EXPECT_EQ(circularRange(value, low, high), low);
}

TEST(UtilsTests, givenTooLowValue_whenCalculatingCircularRange_shouldClamp) {
    const int value = 0;
    EXPECT_EQ(circularRange(value, low, high), high);
}

TEST(UtilsTests, givenNegativeNumber_whenCalculatingRelu_shouldReturn0) { EXPECT_EQ(ReLU(-5), 0); }

TEST(UtilsTests, givenPositiveNumber_whenCalculatingRelu_shouldReturnSameNumber) {
    const int value = 5;
    EXPECT_EQ(ReLU(value), value);
}

}  // namespace
