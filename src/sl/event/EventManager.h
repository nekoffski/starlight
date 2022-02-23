#pragma once

#include <kc/event/EventEmitter.h>
#include <kc/event/EventEngine.h>

#include <kc/core/Singleton.hpp>

namespace sl::event {

class EventManager : public kc::core::Singleton<EventManager> {
   public:
    explicit EventManager() : m_eventEmitter(m_eventEngine.createEmitter()) {}

    void update() { m_eventEngine.spreadEvents(); }

    void registerListener(kc::event::EventListener* listener) {
        m_eventEngine.registerEventListener(listener);
    }

    void unregisterListener(kc::event::EventListener* listener) {
        m_eventEngine.unregisterEventListener(listener);
    }

    // clang-format off
    template <typename Ev, typename... Args> requires std::derived_from<Ev, kc::event::Event>
    auto emit(Args&&... args) {
        return m_eventEmitter->emit<Ev>(std::forward<Args>(args)...);
    }
    // clang-format on

   private:
    kc::event::EventEngine m_eventEngine;
    std::shared_ptr<kc::event::EventEmitter> m_eventEmitter;
};

}  // namespace sl::event