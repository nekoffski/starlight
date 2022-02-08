#include "sl/async/AsyncManager.hpp"

#include <gtest/gtest.h>

using namespace sl;
using namespace testing;

struct FakePeriodicTask : async::PeriodicTask {
    void invoke() override {
        ++s_invoked;
    }

    bool shouldInvoke() override {
        return s_shouldInvoke;
    }

    std::string getName() const override {
        return "__";
    }

    static void reset() {
        s_shouldInvoke = false;
        s_invoked = 0;
    }

    inline static bool s_shouldInvoke = false;
    inline static int s_invoked = 0;
};

struct FakeAsyncTask : async::AsyncTask {
    void executeAsync() override {
        s_asyncInvoked = true;
    }

    void finalize() override {
        s_finalized = true;
    }

    std::string asString() const override {
        return "__";
    }

    static void reset() {
        s_finalized = false;
        s_asyncInvoked = false;
    }

    inline static bool s_finalized = false;
    inline static bool s_asyncInvoked = false;
};

struct AsyncManagerTests : Test {
    void SetUp() override {
        FakePeriodicTask::reset();
        FakeAsyncTask::reset();
    }

    async::AsyncManager asyncManager;

    static constexpr float dt = 0.1f;
    static constexpr int microsecondsInSecond = 1000000;
};

TEST_F(AsyncManagerTests, givenAsyncManager_whenInsertingPeriodicTasks_shouldBeCalledBasedOnWhatShouldInvokeReturns) {
    asyncManager.addPeriodicTask<FakePeriodicTask>();

    ASSERT_EQ(FakePeriodicTask::s_invoked, 0);

    FakePeriodicTask::s_shouldInvoke = true;
    asyncManager.update(dt);
    asyncManager.update(dt);

    FakePeriodicTask::s_shouldInvoke = false;
    asyncManager.update(dt);

    EXPECT_EQ(FakePeriodicTask::s_invoked, 2);
}

TEST_F(AsyncManagerTests, givenAsyncManager_whenDisablingPeriodicTask_shouldBeCalledNoMore) {
    auto guard = asyncManager.addPeriodicTask<FakePeriodicTask>();

    ASSERT_EQ(FakePeriodicTask::s_invoked, 0);

    FakePeriodicTask::s_shouldInvoke = true;
    asyncManager.update(dt);
    asyncManager.update(dt);

    ASSERT_EQ(FakePeriodicTask::s_invoked, 2);
    ASSERT_EQ(FakePeriodicTask::s_shouldInvoke, true);

    guard.disable();

    asyncManager.update(dt);
    asyncManager.update(dt);

    EXPECT_EQ(FakePeriodicTask::s_invoked, 2);
}

TEST_F(AsyncManagerTests, givenAsyncManager_whenCallingFunctionAsync_shouldBeCalled) {
    static constexpr int result = 1337;
    static constexpr int delta = 1;

    auto future = asyncManager.callAsync([&] {
        return result + delta;
    });

    EXPECT_EQ(future.getValue(), result + delta);
}

TEST_F(AsyncManagerTests, givenAsyncManager_whenCallingAsyncTask_shouldInvokeAsyncAndSyncParts) {
    ASSERT_FALSE(FakeAsyncTask::s_asyncInvoked);
    ASSERT_FALSE(FakeAsyncTask::s_finalized);

    asyncManager.callAsync<FakeAsyncTask>();

    int iterations = 0;
    while (not FakeAsyncTask::s_asyncInvoked) {
        if (iterations++ > 25)
            FAIL() << "Async part of Task was not called";
        usleep(microsecondsInSecond / 100); // ugly, I know :(
    }

    ASSERT_TRUE(FakeAsyncTask::s_asyncInvoked);
    ASSERT_FALSE(FakeAsyncTask::s_finalized);

    asyncManager.update(dt);

    EXPECT_TRUE(FakeAsyncTask::s_finalized);
}

TEST_F(AsyncManagerTests, givenTimer_whenTimeExceedSleepTime_shouldReturnFalse) {
    static constexpr float sleepTime = 10.0f;
    auto timer = asyncManager.createTimer(sleepTime);

    ASSERT_TRUE(timer->asyncSleep());

    REPEAT(4) {
        asyncManager.update(1.0f);
        ASSERT_TRUE(timer->asyncSleep());
    }

    asyncManager.update(sleepTime);

    ASSERT_FALSE(timer->asyncSleep());
    EXPECT_TRUE(timer->asyncSleep()); // previous asyncSleep should reset state
}
