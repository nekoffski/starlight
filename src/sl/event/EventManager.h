#pragma once

#include <xvent/EventEmitter.h>
#include <xvent/EventEngine.h>

#include "sl/core/Singleton.hpp"

namespace sl::event {

class EventManager : public core::Singleton<EventManager> {
public:
    explicit EventManager()
        : m_eventEmitter(m_eventEngine.createEmitter()) {
    }

    void update() {
        m_eventEngine.spreadEvents();
    }

    void registerListener(xvent::EventListener* listener) {
        m_eventEngine.registerEventListener(listener);
    }

    void unregisterListener(xvent::EventListener* listener) {
        m_eventEngine.unregisterEventListener(listener);
    }

    // clang-format off
    template <typename Ev, typename... Args> requires std::derived_from<Ev, xvent::Event>
    auto emit(Args&&... args) {
        return m_eventEmitter->emit<Ev>(std::forward<Args>(args)...);
    }
    // clang-format on

private:
    xvent::EventEngine m_eventEngine;
    std::shared_ptr<xvent::EventEmitter> m_eventEmitter;
};

}