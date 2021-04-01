#pragma once

#include <mutex>
#include <optional>
#include <queue>

namespace sl::core {

template <typename T>
class AtomicQueue {
public:
    std::size_t size() {
        const std::lock_guard guard { m_queueMutex };
        return m_queue.size();
    }

    bool isEmpty() {
        const std::lock_guard guard { m_queueMutex };
        return m_queue.empty();
    }

    void enqueue(T&& value) {
        const std::lock_guard guard { m_queueMutex };
        m_queue.push(value);
    }

    void enqueue(const T& value) {
        const std::lock_guard guard { m_queueMutex };
        m_queue.push(value);
    }

    std::optional<T> dequeue() {
        const std::lock_guard guard { m_queueMutex };
        if (m_queue.empty())
            return std::nullopt;

        auto item = m_queue.front();
        m_queue.pop();
        return item;
    }

private:
    std::queue<T> m_queue;
    std::mutex m_queueMutex;
};
}
