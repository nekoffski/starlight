#pragma once

#include <memory>

#include "impl/ClockImpl.h"

namespace sl::platform::time {

class Clock {
public:
    template <typename T>
    static void setClockImpl() {
        m_pimpl = std::make_unique<T>();
    }

    static float getDeltaTime() {
        return m_pimpl->getDeltaTime();
    }

    static void update() {
        m_pimpl->update();
    }

    static std::shared_ptr<impl::ITimestamp> now() {
        return m_pimpl->now();
    }

private:
    static inline std::unique_ptr<impl::IClockImpl> m_pimpl;
};
}

// #pragma once

// #include <memory>

// #include "tickers/NullTicker.h"
// #include "tickers/Ticker.h"

// namespace sl::platform::clock {

// class Clock {
//     class Timestamp {
//         friend class Clock;

//     public:
//         float getTime() const;

//         operator float() {
//             return getTime();
//         }

//         Timestamp operator-(const Timestamp& rhs) {
//             return Timestamp(m_ticksTimestamp - rhs.m_ticksTimestamp);
//         }

//     private:
//         explicit Timestamp(float);
//         float m_ticksTimestamp;
//     };

// public:
//     Timestamp now();

//     void update();
//     float getDeltaTime();

//     template <TickerPolicy T>
//     void setTickerPolicy() {
//         m_ticker = std::make_unique<T>();
//     }

// private:
//     inline static std::unique_ptr<tickers::Ticker> m_ticker =
//         std::make_unique<tickers::NullTicker>();

//     float m_previous{ 0.0f };
//     float m_current{ 0.0f };
//     float m_delta{ 0.0f };
// };
// }
