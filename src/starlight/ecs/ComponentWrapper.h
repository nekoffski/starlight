#pragma once

#include <memory>

#include "starlight/gui/Window.h"

namespace sl::ecs {

class Component;

class ComponentWrapper {
public:
    explicit ComponentWrapper(Component& component)
        : m_component(component) {
    }

    virtual void onGUI(gui::Window& window) = 0;

protected:
    Component& m_component;
};

struct ComponentWrapperFactory {
    virtual std::unique_ptr<ComponentWrapper> create(Component&) = 0;
};

struct NullComponentWrapper : ComponentWrapper {
    using ComponentWrapper::ComponentWrapper;

    void onGUI(gui::Window& window) override {
    }
};
}
