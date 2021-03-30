#pragma once

#include "sl/geom/Mesh.h"
#include "sl/geom/Model.h"
#include "sl/gfx/fwd.h"
#include "sl/platform/fwd.h"

#include "sl/ecs/ComponentView.hpp"
#include "sl/gfx/data/ModelData.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/RendererComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::gfx::renderer {

class ModelRenderer {
public:
    explicit ModelRenderer(std::shared_ptr<gfx::LowLevelRenderer> renderer);

    void render(scene::components::RendererComponent& component, ecs::ComponentView<scene::components::MaterialComponent>, ecs::ComponentView<scene::components::ModelComponent>,
        ecs::ComponentView<scene::components::TransformComponent>, std::shared_ptr<gfx::camera::Camera> camera);

    void render(scene::components::RendererComponent& component, ecs::ComponentView<scene::components::MaterialComponent>, ecs::ComponentView<scene::components::ModelComponent>,
        ecs::ComponentView<scene::components::TransformComponent>, std::shared_ptr<gfx::camera::Camera> camera, std::shared_ptr<gfx::Shader>);

private:
    void setMaterial(const scene::components::MaterialComponent& material, std::shared_ptr<gfx::Shader> shader);

    void renderModelComposite(std::shared_ptr<gfx::Shader> shader, const gfx::data::ModelData& modelData,
        const math::Mat4& transform, std::shared_ptr<gfx::camera::Camera>);

    void renderModel(std::shared_ptr<geom::Model>);

    void renderMesh(std::shared_ptr<geom::Mesh>);

    std::shared_ptr<gfx::LowLevelRenderer> m_renderer;
};
}
