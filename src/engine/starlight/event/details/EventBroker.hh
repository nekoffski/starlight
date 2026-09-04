#pragma once

#include <any>
#include <mutex>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Functional.hh"
#include "starlight/core/Log.hh"

namespace sl::details {

class EventBroker : public NonCopyable, public NonMovable {
    using GenericCallback = MoveOnlyFunction<void(const std::any&)>;
    using CallbackMap = std::unordered_map<
        std::type_index, std::unordered_map<Str, GenericCallback>>;
    using EventMap = std::unordered_map<std::type_index, std::vector<std::any>>;

   public:
    void dispatch();

    template <typename T>
    void post(const T& event) {
        post(std::type_index(typeid(T)), std::any{event});
    }

    template <typename T>
    void registerCallback(
        const Str& id, MoveOnlyFunction<void(const T&)> callback
    ) {
        registerCallback(
            id, typeid(T), [cb = std::move(callback)](const std::any& event) {
                if (const auto* v = std::any_cast<T>(&event)) [[likely]] {
                    cb(*v);
                }
            }
        );
    }

    void unregisterCallback(const Str& id, const std::type_index& type);
    void unregister(const Str& id);

   private:
    void post(const std::type_index& type, const std::any& event);

    EventMap consume();

    void registerCallback(
        const Str& id, const std::type_index& type, GenericCallback&& callback
    );

    CallbackMap m_callbacks;
    EventMap m_events;

    std::mutex m_handlerMutex;
    std::mutex m_eventMutex;
};

}  // namespace sl::details
