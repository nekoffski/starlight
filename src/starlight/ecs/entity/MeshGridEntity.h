#pragma once

#include "Entity.h"

#include "starlight/ecs/component/MeshGridComponent.h"
#include "starlight/geometry/Geometry.hpp"

namespace sl::ecs::entity {

class MeshGridEntity : public Entity {
public:
    explicit MeshGridEntity(std::string name, std::shared_ptr<platform::shader::Shader> shader)
        : Entity(std::move(name))
        , m_gridRenderData(std::make_shared<rendering::data::ModelRenderData>(
              geometry::Geometry::getSquare()))
        , m_shader(shader) {

        addComponent(
            std::make_shared<component::ModelComponent>(m_gridRenderData, m_shader));
        addComponent(
            std::make_shared<component::MeshGridComponent>(m_gridRenderData));
    }

private:
    std::shared_ptr<rendering::data::ModelRenderData> m_gridRenderData;
    std::shared_ptr<platform::shader::Shader> m_shader;
};
}