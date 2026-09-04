#pragma once

#include "EventBus.hh"
#include "details/EventBroker.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Functional.hh"

namespace sl {

class EventSystem : public NonCopyable, public NonMovable {
   public:
    explicit EventSystem();

    EventBus createBus();
    void dispatch();

   private:
    std::shared_ptr<details::EventBroker> m_broker;
};

}  // namespace sl
