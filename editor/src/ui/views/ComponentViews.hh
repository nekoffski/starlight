#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>

#include <starlight/core/Concepts.hh>

namespace sle {

class ComponentViews {
    using Callback = std::function<void(void*)>;

public:
    explicit ComponentViews();

    void render(std::type_index index, void* component);

private:
    template <typename T, typename F>
    requires sl::Callable<F, void, T&>
    void registerView(F&& callback) {
        m_views[typeid(T)] = [callback = std::move(callback)](void* component) {
            callback(*static_cast<T*>(component));
        };
    }

    std::unordered_map<std::type_index, Callback> m_views;
};

}  // namespace sle
