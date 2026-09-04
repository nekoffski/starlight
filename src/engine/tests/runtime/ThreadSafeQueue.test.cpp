#include "starlight/runtime/ThreadSafeQueue.hh"

#include <gtest/gtest.h>

#include <chrono>
#include <future>
#include <memory>

namespace {

TEST(ThreadSafeQueue, PopsValuesInFirstInFirstOutOrder) {
    sl::ThreadSafeQueue<int> queue;

    ASSERT_TRUE(queue.push(1));
    ASSERT_TRUE(queue.push(2));

    const auto first = queue.tryPop();
    const auto second = queue.tryPop();

    ASSERT_TRUE(first.has_value());
    ASSERT_TRUE(second.has_value());
    EXPECT_EQ(*first, 1);
    EXPECT_EQ(*second, 2);
}

TEST(ThreadSafeQueue, TryPopReturnsEmptyWhenNoValueIsAvailable) {
    sl::ThreadSafeQueue<int> queue;

    EXPECT_FALSE(queue.tryPop().has_value());
}

TEST(ThreadSafeQueue, PopWaitsUntilAValueIsPushed) {
    sl::ThreadSafeQueue<int> queue;

    auto popped =
        std::async(std::launch::async, [&queue] { return queue.pop(); });

    EXPECT_EQ(
        popped.wait_for(std::chrono::milliseconds{100}),
        std::future_status::timeout
    );

    ASSERT_TRUE(queue.push(42));

    ASSERT_EQ(
        popped.wait_for(std::chrono::seconds{1}), std::future_status::ready
    );
    const auto value = popped.get();
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(*value, 42);
}

TEST(ThreadSafeQueue, CloseWakesWaitingPopWithoutAValue) {
    sl::ThreadSafeQueue<int> queue;

    auto popped =
        std::async(std::launch::async, [&queue] { return queue.pop(); });

    EXPECT_EQ(
        popped.wait_for(std::chrono::milliseconds{100}),
        std::future_status::timeout
    );

    queue.close();

    ASSERT_EQ(
        popped.wait_for(std::chrono::seconds{1}), std::future_status::ready
    );
    EXPECT_FALSE(popped.get().has_value());
}

TEST(ThreadSafeQueue, DrainsQueuedValuesAfterClosureAndRejectsNewOnes) {
    sl::ThreadSafeQueue<int> queue;

    ASSERT_TRUE(queue.push(1));
    ASSERT_TRUE(queue.push(2));

    queue.close();

    EXPECT_TRUE(queue.closed());
    EXPECT_FALSE(queue.push(3));

    const auto first = queue.pop();
    const auto second = queue.pop();
    const auto afterDrain = queue.pop();

    ASSERT_TRUE(first.has_value());
    ASSERT_TRUE(second.has_value());
    EXPECT_EQ(*first, 1);
    EXPECT_EQ(*second, 2);
    EXPECT_FALSE(afterDrain.has_value());
}

TEST(ThreadSafeQueue, SupportsMoveOnlyValues) {
    sl::ThreadSafeQueue<std::unique_ptr<int>> queue;
    auto value = std::make_unique<int>(42);

    ASSERT_TRUE(queue.push(std::move(value)));

    const auto popped = queue.tryPop();

    ASSERT_TRUE(popped.has_value());
    ASSERT_NE(*popped, nullptr);
    EXPECT_EQ(**popped, 42);
}

}  // namespace
