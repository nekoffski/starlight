#pragma once

#include <memory>

#include <starlight/ecs/component/Component.h>
#include <starlight/math/Vector.hpp>

namespace starl::ecs::entity {

class Entity {
public:
    std::vector<std::shared_ptr<component::Component>>& getComponents() {
        return m_components;
    }

    std::vector<std::shared_ptr<Entity>>& getChilds() {
        return m_childs;
    }

    void addComponent(std::shared_ptr<component::Component> component) {
        m_components.push_back(std::move(component));
    }

private:
    math::Vec3 m_position;
    std::vector<std::shared_ptr<component::Component>> m_components;
    std::vector<std::shared_ptr<Entity>> m_childs;
};
}