#pragma once

#include <memory>
#include <string>

#include "ClockImpl.h"
#include "Macros.h"

namespace sl::core {

class Clock {
    SL_SINGLETON(Clock);

public:
    template <typename T>
    void setClockImpl() {
        m_pimpl = std::make_unique<T>();
    }

    float getDeltaTime() {
        return m_pimpl->getDeltaTime();
    }

    void update() {
        m_pimpl->update();
    }

    std::string getTimeString(const std::string& format) {
        return m_pimpl->getTimeString(format);
    }

    std::shared_ptr<Timestamp> now() {
        return m_pimpl->now();
    }

private:
    std::unique_ptr<ClockImpl> m_pimpl;
};
}

#define CLOCK() sl::core::Clock::instance()
