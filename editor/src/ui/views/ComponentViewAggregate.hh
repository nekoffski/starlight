#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>

#include <starlight/core/Concepts.hh>

#include "ui/Widget.hh"
#include "components/ComponentView.hh"

namespace sle {

class ComponentViewAggregate : public Widget {
public:
    explicit ComponentViewAggregate(Widget::State& state);

    void render(std::type_index index, void* component);

private:
    template <typename View>
    requires std::is_base_of_v<ComponentView, View>
    void registerView() {
        m_views[typeid(typename View::Type)] = std::make_unique<View>(getState());
    }

    std::unordered_map<std::type_index, std::unique_ptr<ComponentView>> m_views;
};

}  // namespace sle
