#pragma once

#include "sl/geometry/Mesh.h"
#include "sl/geometry/Model.h"
#include "sl/graphics/fwd.h"
#include "sl/platform/fwd.h"

#include "sl/ecs/ComponentView.hpp"
#include "sl/graphics/data/ModelData.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/RendererComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::scene::systems {

class RendererSystem {
public:
    explicit RendererSystem(std::shared_ptr<graphics::Renderer> renderer);

    void render(components::RendererComponent& component, ecs::ComponentView<components::MaterialComponent>&, ecs::ComponentView<components::ModelComponent>&,
        ecs::ComponentView<components::TransformComponent>&, std::shared_ptr<graphics::camera::Camera> camera);

    void render(components::RendererComponent& component, ecs::ComponentView<components::MaterialComponent>&, ecs::ComponentView<components::ModelComponent>&,
        ecs::ComponentView<components::TransformComponent>&, std::shared_ptr<graphics::camera::Camera> camera, std::shared_ptr<graphics::Shader>);

private:
    void setMaterial(const components::MaterialComponent& material, std::shared_ptr<graphics::Shader> shader);

    void renderModelComposite(std::shared_ptr<graphics::Shader> shader, const graphics::data::ModelData& modelData,
        const math::Mat4& transform);

    void renderModel(std::shared_ptr<geometry::Model>);

    void renderMesh(std::shared_ptr<geometry::Mesh>);

    std::shared_ptr<graphics::Renderer> m_renderer;
};
}
