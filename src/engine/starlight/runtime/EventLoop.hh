#pragma once

#include <atomic>
#include <kcore/Concepts.hh>
#include <kcore/Functional.hh>
#include <kcore/Random.hh>
#include <kcore/containers/FlatMap.hh>
#include <kcore/runtime/Thread.hh>
#include <memory>
#include <mutex>

#include "starlight/core/Core.hh"
#include "starlight/core/Time.hh"

namespace sl {

enum class EventLoopCallState { pendingReschedule, waiting, done, failed };

class EventLoopCall : virtual public kc::NonCopyable,
                      virtual public kc::NonMovable {
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

    kc::MoveOnlyFunction<bool()> m_poll;
};

class EventLoop : public kc::Thread {
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
    kc::FlatMap<Str, std::unique_ptr<EventLoopCall>> m_calls;
    std::mutex m_callsMutex;
};

}  // namespace sl
