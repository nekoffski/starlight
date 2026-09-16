#include "EventLoop.hh"

#include "starlight/core/Time.hh"

namespace sl {

EventLoop::EventLoop(const Str& name) : Thread(name) {}

EventLoop::~EventLoop() { stop(); }

void EventLoop::stop() { m_running = false; }

void EventLoop::remove(const Tag<Str>& uuid) { m_calls.remove(uuid.get()); }

Tag<Str> EventLoop::scheduleImpl(std::unique_ptr<EventLoopCall> call) {
    const auto uuid = RandomEngine::get().uuid();
    m_calls.insert(uuid, std::move(call));
    return Tag<Str>{uuid};
}

void EventLoop::run() {
    while (m_running) {
        bool anyReady = false;

        const auto now = Clock::now();
        std::vector<Str> dropList;

        {
            std::lock_guard lk{m_callsMutex};

            m_calls.forEach([&](const auto& uuid, auto& call) {
                const auto state = call->execute(now);

                if (state == EventLoopCallState::done) {
                    dropList.push_back(uuid);
                }
            });

            m_calls.remove(dropList);
        }

        if (not anyReady) {
            sleepFor(100ms);
        }
    }
}

PollingEventLoopCallBase::PollingEventLoopCallBase(
    const std::chrono::milliseconds& interval
)
    : m_interval(interval), m_nextDeadline(Clock::now() + interval) {}

EventLoopCallState PollingEventLoopCallBase::execute(
    const Clock::time_point& now
) {
    if (not shouldCall(now)) {
        return EventLoopCallState::waiting;
    }
    return poll() ? EventLoopCallState::done
                  : EventLoopCallState::pendingReschedule;
}

bool PollingEventLoopCallBase::shouldCall(Clock::time_point now) {
    auto deadlineMet = now >= m_nextDeadline;
    if (deadlineMet) {
        m_nextDeadline = now + m_interval;
    }
    return deadlineMet;
}

}  // namespace sl
