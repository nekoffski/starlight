#pragma once

#include "sl/geometry/Mesh.h"
#include "sl/geometry/Model.h"
#include "sl/platform/fwd.h"
#include "sl/rendering/fwd.h"

#include "sl/rendering/data/ModelData.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/RendererComponent.h"

namespace sl::scene::systems {

class RendererSystem {
public:
    explicit RendererSystem(std::shared_ptr<rendering::Renderer> renderer);
    void render(components::RendererComponent& component, std::shared_ptr<rendering::camera::Camera> camera);
    void render(components::RendererComponent& component, std::shared_ptr<rendering::camera::Camera> camera,
        std::shared_ptr<platform::shader::Shader>);

private:
    void setMaterial(const components::MaterialComponent& material, std::shared_ptr<platform::shader::Shader> shader);
    void renderModelComposite(std::shared_ptr<platform::shader::Shader> shader, const rendering::data::ModelData& modelData,
        const math::Mat4& transform);
    void renderModel(std::shared_ptr<geometry::Model>);
    void renderMesh(std::shared_ptr<geometry::Mesh>);

    std::shared_ptr<rendering::Renderer> m_renderer;
};
}
