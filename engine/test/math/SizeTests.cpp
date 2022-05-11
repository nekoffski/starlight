#include "nova/math/Size.hpp"

#include <gtest/gtest.h>

using namespace nova;

TEST(SizeTests, givenSize2f_whenCastingToVector_shouldGiveCorrectVector) {
    math::Size2f s{1.0f, -5.0f};
    glm::vec2 sv = s;

    EXPECT_EQ(sv.x, s.width);
    EXPECT_EQ(sv.y, s.height);
}

TEST(SizeTests, givenSize2i_whenCastingToVector_shouldGiveCorrectVector) {
    math::Size2i s{1, -5};
    glm::vec2 sv = s;

    EXPECT_EQ(sv.x, s.width);
    EXPECT_EQ(sv.y, s.height);
}
