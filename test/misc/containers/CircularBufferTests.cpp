#include <memory>

#include <gtest/gtest.h>

#include "sl/misc/containers/CircularBuffer.hpp"

using namespace sl::misc::containers;

TEST(CircularBuffer, givenCircularBuffer_expectCorrectSize) {
    const int size = 100;
    CircularBuffer<int, size> m;
    ASSERT_EQ(m.maxSize(), size);
}
