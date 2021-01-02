#pragma once

#include <cstdint>

#include <array>
#include <functional>
#include <optional>

namespace sl::core::containers {

template <class T, std::size_t N>
class CircularBuffer {
    using Callback = std::function<void(T&)>;

public:
    explicit CircularBuffer()
        : m_begin(0) {

        for (int i = 0; i < N; ++i)
            m_buffer[i] = {};
    }

    void push(T value) {
        m_buffer[m_begin++ % (N - 1)] = std::move(value);
    }

    void foreach (Callback callback) {
        for (auto& val : m_buffer)
            if (val)
                callback(val.value());
    }

    std::size_t maxSize() const {
        return N;
    }

private:
    std::size_t m_begin;
    std::array<std::optional<T>, N> m_buffer;
};
}
