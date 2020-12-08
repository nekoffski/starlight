#pragma once

#include <memory>

namespace sl::gui {
class GuiProxy;
}

namespace sl::ecs {

class Component;

class ComponentWrapper {
public:
    explicit ComponentWrapper(Component& component)
        : m_component(component) {
    }

    virtual void onGui(gui::GuiProxy&) = 0;

protected:
    Component& m_component;
};

struct ComponentWrapperFactory {
    virtual std::unique_ptr<ComponentWrapper> create(Component&) = 0;
};

struct NullComponentWrapper : ComponentWrapper {
    using ComponentWrapper::ComponentWrapper;

    void onGui(gui::GuiProxy&) override {
    }
};
}
