#pragma once

#include <functional>
#include <memory>

#include <starlight/core/event/Event.h>

namespace starl::core::event {

class EventDispatcher {
public:
    explicit EventDispatcher(std::shared_ptr<Event> e) : m_event(e) {
    }

    template <typename T>
    void setCallback(std::function<bool(std::shared_ptr<T>)> callback) {
        if (isInstanceOf<T>(*m_event)) {
            auto isHandled = callback(std::dynamic_pointer_cast<T>(m_event));
            if (m_event->dispatchOnce) {
                const_cast<bool&>(m_event->isHandled) = isHandled;
            }
        }
    }

private:
    std::shared_ptr<Event> m_event;

    template <typename Base, typename T>
    static inline bool isInstanceOf(const T& t) {
        return typeid(t) == typeid(Base);
    }
};

}