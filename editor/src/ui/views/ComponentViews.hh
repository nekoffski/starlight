#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>

#include <starlight/core/Concepts.hh>

#include "ui/Widget.hh"

namespace sle {

class ComponentViews : public Widget {
    using Callback = std::function<void(void*, Widget&)>;

public:
    explicit ComponentViews(Widget::State& state);

    void render(std::type_index index, void* component);

private:
    template <typename T, typename F>
    requires sl::Callable<F, void, T&, Widget&>
    void registerView(F&& callback) {
        m_views[typeid(T)] = [callback](void* component, Widget& widget) {
            callback(*static_cast<T*>(component), widget);
        };
    }

    std::unordered_map<std::type_index, Callback> m_views;
};

}  // namespace sle
