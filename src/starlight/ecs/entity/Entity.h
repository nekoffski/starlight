#pragma once

#include <memory>

#include <starlight/ecs/component/Component.h>
#include <starlight/gui/Window.h>
#include <starlight/math/Vector.hpp>

namespace starl::ecs::entity {

class Entity {
public:
    explicit Entity(std::string name)
        : m_name(std::move(name)) {
    }

    const std::string& getName() {
        return m_name;
    }

    void setPosition(math::Vec3 position) {
        m_position = std::move(position);
    }

    math::Vec3 getPosition() {
        return m_position;
    }

    void setOrientation(math::Vec3 orientation) {
        m_orientation = std::move(orientation);
    }

    math::Vec3 getOrientation() {
        return m_orientation;
    }

    std::vector<std::shared_ptr<component::Component>>& getComponents() {
        return m_components;
    }

    std::vector<std::shared_ptr<Entity>>& getChilds() {
        return m_childs;
    }

    void addComponent(std::shared_ptr<component::Component> component) {
        m_components.push_back(std::move(component));
    }

    void onGUI(gui::Window& window) {
        for (const auto& component : m_components)
            component->onGUI(window);
    }

private:
    std::string m_name;
    math::Vec3 m_position;
    math::Vec3 m_orientation;
    std::vector<std::shared_ptr<component::Component>> m_components;
    std::vector<std::shared_ptr<Entity>> m_childs;
};
}