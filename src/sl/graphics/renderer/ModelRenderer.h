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

namespace sl::graphics::renderer {

class ModelRenderer {
public:
    explicit ModelRenderer(std::shared_ptr<graphics::LowLevelRenderer> renderer);

    void render(scene::components::RendererComponent& component, ecs::ComponentView<scene::components::MaterialComponent>, ecs::ComponentView<scene::components::ModelComponent>,
        ecs::ComponentView<scene::components::TransformComponent>, std::shared_ptr<graphics::camera::Camera> camera);

    void render(scene::components::RendererComponent& component, ecs::ComponentView<scene::components::MaterialComponent>, ecs::ComponentView<scene::components::ModelComponent>,
        ecs::ComponentView<scene::components::TransformComponent>, std::shared_ptr<graphics::camera::Camera> camera, std::shared_ptr<graphics::Shader>);

private:
    void setMaterial(const scene::components::MaterialComponent& material, std::shared_ptr<graphics::Shader> shader);

    void renderModelComposite(std::shared_ptr<graphics::Shader> shader, const graphics::data::ModelData& modelData,
        const math::Mat4& transform, std::shared_ptr<graphics::camera::Camera>);

    void renderModel(std::shared_ptr<geometry::Model>);

    void renderMesh(std::shared_ptr<geometry::Mesh>);

    std::shared_ptr<graphics::LowLevelRenderer> m_renderer;
};
}
