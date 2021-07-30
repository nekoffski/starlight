#pragma once

#include <memory>

#include "sl/ecs/ComponentView.hpp"
#include "sl/gfx/Renderer.h"
#include "sl/gfx/VertexArray.h"
#include "sl/scene/components/ParticleEffectComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::gfx::renderer {

class ParticleEffectRenderer {
public:
    explicit ParticleEffectRenderer(Renderer& renderer);

    void renderParticleEffects(ecs::ComponentView<scene::components::ParticleEffectComponent> pfxs,
        ecs::ComponentView<scene::components::TransformComponent> transforms, camera::Camera& camera);

private:
    void beginParticleEffect(gfx::camera::Camera&);
    void renderParticle();
    void endParticleEffect();

    std::shared_ptr<gfx::VertexArray> m_vao;

    std::shared_ptr<gfx::Shader> m_shader;
    Renderer& m_renderer;
};
}
