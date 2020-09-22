#pragma once

#include "sl/rendering/fwd.h"

#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/RendererComponent.h"

namespace sl::scene::systems {

class RendererSystem {
public:
    explicit RendererSystem(std::shared_ptr<rendering::RendererProxy> renderer);
    void render(components::RendererComponent& component, std::shared_ptr<rendering::camera::Camera> camera);

private:
    void setMaterial(const components::MaterialComponent& material, std::shared_ptr<platform::shader::Shader> shader);
    std::shared_ptr<rendering::RendererProxy> m_renderer;
};
}
