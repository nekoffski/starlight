#include "sl/core/AtomicQueue.hpp"

#include <gtest/gtest.h>

using namespace testing;
using namespace sl::core;

namespace {

class AtomicQueueTests : public Test {
public:
    AtomicQueue<int> m_atomicQueue;
};

TEST_F(AtomicQueueTests, givenEmptyQueue_whenAskingForSize_shouldReturn0) {
    EXPECT_EQ(m_atomicQueue.size(), 0);
}

TEST_F(AtomicQueueTests, givenEmptyQueue_whenCheckingIfIsEmpty_shouldReturnTrue) {
    EXPECT_TRUE(m_atomicQueue.isEmpty());
}

TEST_F(AtomicQueueTests, givenNonEmptyQueue_whenAskingForSize_shouldReturnSize) {
    m_atomicQueue.enqueue(1);
    EXPECT_EQ(m_atomicQueue.size(), 1);

    m_atomicQueue.enqueue(1);
    EXPECT_EQ(m_atomicQueue.size(), 2);
}

TEST_F(AtomicQueueTests, givenNonEmptyQueue_whenCheckingIfIsEmpty_shouldReturnFalse) {
    m_atomicQueue.enqueue(1);
    EXPECT_FALSE(m_atomicQueue.isEmpty());
}

TEST_F(AtomicQueueTests, givenEmptyQueue_whenDequingFromEmptyQueue_shouldReturnNullOptional) {
    auto val = m_atomicQueue.dequeue();
    EXPECT_FALSE(val.has_value());
}

TEST_F(AtomicQueueTests, givenNonEmptyQueue_whenDequingFromEmptyQueue_shouldReturnValue) {
    const int input = 5;
    m_atomicQueue.enqueue(input);

    auto val = m_atomicQueue.dequeue();

    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(val.value(), input);
}
}
