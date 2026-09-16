#pragma once

#include <atomic>
#include <memory>
#include <mutex>

#include "Thread.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/FlatMap.hh"
#include "starlight/core/Functional.hh"
#include "starlight/core/Random.hh"
#include "starlight/core/Time.hh"

namespace sl {

enum class EventLoopCallState { pendingReschedule, waiting, done, failed };

class EventLoopCall : virtual public NonCopyable, virtual public NonMovable {
   public:
    virtual ~EventLoopCall() = default;

    virtual EventLoopCallState execute(const Clock::time_point& now) = 0;
};

class PollingEventLoopCallBase : public EventLoopCall {
   public:
    explicit PollingEventLoopCallBase(
        const std::chrono::milliseconds& interval
    );

   private:
    EventLoopCallState execute(const Clock::time_point& now) override;

    virtual bool poll() = 0;
    virtual bool shouldCall(Clock::time_point now);

    std::chrono::milliseconds m_interval;
    Clock::time_point m_nextDeadline;
};

class PollingEventLoopCall : public PollingEventLoopCallBase {
   public:
    explicit PollingEventLoopCall(
        const std::chrono::milliseconds& interval, auto&& f
    )
        : PollingEventLoopCallBase(interval),
          m_poll(std::forward<decltype(f)>(f)) {}

    explicit PollingEventLoopCall(auto&& f)
        : PollingEventLoopCallBase(50ms),
          m_poll(std::forward<decltype(f)>(f)) {}

   private:
    bool poll() override { return m_poll(); }

    MoveOnlyFunction<bool()> m_poll;
};

class EventLoop : public Thread {
   public:
    explicit EventLoop(const Str& name);
    ~EventLoop();

    void stop();

    template <typename T, typename... Args>
        requires std::is_base_of_v<EventLoopCall, T> &&
                 std::is_constructible_v<T, Args...>
    Tag<Str> schedule(Args&&... args) {
        std::lock_guard<std::mutex> lk{m_callsMutex};
        return scheduleImpl(std::make_unique<T>(std::forward<Args>(args)...));
    }

    void remove(const Tag<Str>& uuid);

   private:
    Tag<Str> scheduleImpl(std::unique_ptr<EventLoopCall> call);

    void run() override;

    std::atomic_bool m_running{true};
    FlatMap<Str, std::unique_ptr<EventLoopCall>> m_calls;
    std::mutex m_callsMutex;
};

}  // namespace sl
