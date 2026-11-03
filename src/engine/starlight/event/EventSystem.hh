#pragma once

#include <kcore/Concepts.hh>
#include <kcore/Functional.hh>

#include "EventBus.hh"
#include "details/EventBroker.hh"
#include "starlight/core/Core.hh"

namespace sl {

class EventSystem : public kc::NonCopyable, public kc::NonMovable {
   public:
    explicit EventSystem();

    EventBus createBus();
    void dispatch();

   private:
    std::shared_ptr<details::EventBroker> m_broker;
};

}  // namespace sl
