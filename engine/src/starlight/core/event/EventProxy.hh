#pragma once
#include "kc/core/Singleton.hpp"

#include "starlight/core/memory/Memory.hh"
#include "starlight/core/Log.hh"
#include "Core.hh"
#include "EventHandlerSentinel.hh"

namespace sl {

class EventProxy : public kc::core::Singleton<EventProxy> {
public:
    explicit EventProxy(details::Events& events, details::EventHandlers& handlers);

    template <typename T, typename... Args> void emit(Args&&... args) {
        m_events.push_back(
          createOwningPtr<details::EventStorage<T>>(std::forward<Args>(args)...)
        );
    }

    template <typename T>
    [[nodiscard]] EventHandlerId pushEventHandler(
      std::function<EventChainBehaviour(const T&)>&& handler
    ) {
        // TODO: thread safety
        auto& chain = m_handlers[typeid(T)];

        chain.emplace_back(
          [handler](details::EventStorageBase& event) -> EventChainBehaviour {
              return handler(event.as<T>());
          }
        );

        return chain.back().getId();
    }

    template <typename T>
    void pushEventHandler(
      std::function<EventChainBehaviour(const T&)>&& handler,
      EventHandlerSentinel& sentinel
    ) {
        const auto handlerId = pushEventHandler(std::move(handler));
        storeHandlerId(handlerId, sentinel);
    }

    void popEventHandler(const EventHandlerId id);

private:
    void storeHandlerId(const EventHandlerId& id, EventHandlerSentinel& sentinel);

    details::Events& m_events;
    details::EventHandlers& m_handlers;
};

}  // namespace sl
