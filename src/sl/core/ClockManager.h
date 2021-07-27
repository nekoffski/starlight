#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "Singleton.hpp"

using namespace std::literals::chrono_literals;

namespace sl::core {

class ClockManager : public Singleton<ClockManager> {
    using ClockType = std::chrono::steady_clock;

    inline static constexpr float microsecondsInSecond = 1000 * 1000;

public:
    using Ptr = std::unique_ptr<ClockManager>;
    using TimePoint = std::chrono::time_point<ClockType>;

    static float toSeconds(const TimePoint& point);

    template <typename U, typename T>
    static float toSeconds(const std::chrono::duration<U, T>& duration) {
        return std::chrono::duration_cast<std::chrono::microseconds>(duration).count() / microsecondsInSecond;
    }

    float getFPS() const;

    float getDeltaTime() const;

    void update();

    std::string getTimeString(const std::string& format) const;

    TimePoint now() const;

    float nowAsFloat() const;

private:
    ClockType m_clock;
    TimePoint m_previousNow;

    float m_deltaTime = 0.0f;
};
}
