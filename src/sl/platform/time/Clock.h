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

    static std::string getTimeString(std::string format) {
        return m_pimpl->getTimeString(std::move(format));
    }

    static std::shared_ptr<impl::ITimestamp> now() {
        return m_pimpl->now();
    }

private:
    static inline std::unique_ptr<impl::IClockImpl> m_pimpl;
};
}
