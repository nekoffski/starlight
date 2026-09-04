#pragma once

#include <concepts>
#include <condition_variable>
#include <mutex>
#include <queue>

#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"

namespace sl {

template <typename T>
    requires std::move_constructible<T>
class ThreadSafeQueue : public NonCopyable, public NonMovable {
   public:
    ThreadSafeQueue() = default;

    [[nodiscard]] bool push(T&& value) {
        std::lock_guard lk{m_mutex};
        if (m_closed) {
            return false;
        }
        m_queue.push(std::move(value));
        m_condition.notify_one();
        return true;
    }

    [[nodiscard]] Opt<T> pop() {
        std::unique_lock lk{m_mutex};
        m_condition.wait(lk, [this] {
            return m_closed || not m_queue.empty();
        });

        if (m_queue.empty()) {
            return {};
        }

        T value = std::move(m_queue.front());
        m_queue.pop();
        return value;
    }

    [[nodiscard]] Opt<T> tryPop() {
        std::lock_guard lk{m_mutex};
        if (m_queue.empty()) {
            return {};
        }
        T value = std::move(m_queue.front());
        m_queue.pop();
        return value;
    }

    [[nodiscard]] bool closed() const {
        std::lock_guard lk{m_mutex};
        return m_closed;
    }

    void close() {
        {
            std::lock_guard lk{m_mutex};
            m_closed = true;
        }
        m_condition.notify_all();
    }

   private:
    std::queue<T> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_condition;
    bool m_closed{false};
};

}  // namespace sl
