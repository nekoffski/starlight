#pragma once

#include <memory>

#include "sl/ecs/ComponentView.hpp"
#include "sl/graphics/buffer/VertexArray.h"
#include "sl/graphics/fwd.h"
#include "sl/platform/fwd.h"
#include "sl/scene/components/ParticleEffectComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::graphics::renderer {

class ParticleEffectRenderer {
public:
    explicit ParticleEffectRenderer(std::shared_ptr<graphics::LowLevelRenderer> renderer);

    void renderParticleEffects(ecs::ComponentView<scene::components::ParticleEffectComponent> pfxs,
        ecs::ComponentView<scene::components::TransformComponent> transforms, std::shared_ptr<graphics::camera::Camera> camera);

private:
    void beginParticleEffect();
    void renderParticle();
    void endParticleEffect();

    std::shared_ptr<graphics::buffer::VertexArray> m_vao;
   
    std::shared_ptr<graphics::Shader> m_shader;
    std::shared_ptr<graphics::LowLevelRenderer> m_renderer;
};
}
