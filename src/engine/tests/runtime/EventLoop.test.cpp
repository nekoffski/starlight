#include "starlight/runtime/EventLoop.hh"

#include <gtest/gtest.h>

#include <future>

namespace {

using namespace std::chrono_literals;

struct CallState {
    std::promise<void> entered;
    std::promise<void> resume;
    std::atomic_bool completed{false};
    std::atomic_bool destroyedAfterCompletion{false};
};

class BlockingCall : public sl::EventLoopCall {
   public:
    explicit BlockingCall(std::shared_ptr<CallState> state)
        : m_state(std::move(state)) {}

    ~BlockingCall() override {
        m_state->destroyedAfterCompletion = m_state->completed.load();
    }

    sl::EventLoopCallState execute(const sl::Clock::time_point&) override {
        const auto state = m_state;
        state->entered.set_value();
        state->resume.get_future().wait();
        state->completed = true;
        return sl::EventLoopCallState::done;
    }

   private:
    std::shared_ptr<CallState> m_state;
};

TEST(EventLoop, JoinsRunningCallbackBeforeDestroyingCalls) {
    const auto state = std::make_shared<CallState>();
    auto entered = state->entered.get_future();
    auto loop = std::make_unique<sl::EventLoop>("EventLoopTest");
    kc::DeferCall resume{[state] { state->resume.set_value(); }};
    loop->schedule<BlockingCall>(state);
    ASSERT_EQ(entered.wait_for(2s), std::future_status::ready);

    auto destruction = std::async(
        std::launch::async, [loop = std::move(loop)]() mutable { loop.reset(); }
    );
    EXPECT_EQ(destruction.wait_for(20ms), std::future_status::timeout);

    state->resume.set_value();
    resume.dismiss();
    destruction.get();
    EXPECT_TRUE(state->completed);
    EXPECT_TRUE(state->destroyedAfterCompletion);
}

}  // namespace
