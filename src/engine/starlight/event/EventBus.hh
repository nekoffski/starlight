#pragma once

#include "details/EventBroker.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Functional.hh"
#include "starlight/core/Random.hh"

namespace sl {

class EventBus : public NonCopyable, public NonMovable {
    friend class EventSystem;

   public:
    ~EventBus();

    template <typename T>
    void on(MoveOnlyFunction<void(const T&)> callback) {
        m_broker->registerCallback(m_uuid, std::move(callback));
    }

    template <typename T, typename... Args>
        requires std::constructible_from<T, Args...>
    void post(Args&&... args) {
        m_broker->post(T{std::forward<Args>(args)...});
    }

    template <typename T>
    void post(const T& event) {
        m_broker->post(event);
    }

   private:
    explicit EventBus(std::shared_ptr<details::EventBroker>);

    Str m_uuid;
    std::shared_ptr<details::EventBroker> m_broker;
};

}  // namespace sl
