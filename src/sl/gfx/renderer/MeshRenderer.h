#pragma once

#include "sl/geom/Mesh.h"
#include "sl/geom/Model.h"
#include "sl/gfx/LowLevelRenderer.h"

#include "sl/ecs/ComponentView.hpp"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::gfx::renderer {

class MeshRenderer {
public:
    explicit MeshRenderer(LowLevelRenderer& renderer);

    void render(scene::components::MeshRendererComponent& component, ecs::ComponentView<scene::components::MaterialComponent>, ecs::ComponentView<scene::components::ModelComponent>,
        ecs::ComponentView<scene::components::TransformComponent>, gfx::camera::Camera& camera);

    void render(scene::components::MeshRendererComponent& component, ecs::ComponentView<scene::components::MaterialComponent>, ecs::ComponentView<scene::components::ModelComponent>,
        ecs::ComponentView<scene::components::TransformComponent>, gfx::camera::Camera& camera, gfx::Shader&);

private:
    void setMaterial(const scene::components::MaterialComponent& material, gfx::Shader& shader);

    void renderModel(gfx::Shader& shader, scene::components::ModelComponent& model,
        const math::Mat4& transform, gfx::camera::Camera&);

    void renderMesh(geom::Mesh&);

    LowLevelRenderer& m_renderer;
};
}
