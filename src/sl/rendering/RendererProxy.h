#pragma once

#include "data/ModelData.h"
#include "renderer/fwd.h"
#include "renderer/lowlevel/LowLevelRenderer.h"
#include "sl/math/Matrix.hpp"
#include "sl/platform/fwd.h"

#include "sl/rendering/renderer/ParticleRenderer.h"

namespace sl::rendering {

class RendererProxy {
public:
    explicit RendererProxy(renderer::lowlevel::LowLevelRenderer& lowLevelRenderer);

    void renderCubemap(std::shared_ptr<platform::texture::Cubemap> cubemap,
        std::shared_ptr<platform::shader::Shader> cubemapShader,
        std::shared_ptr<rendering::camera::Camera> camera);

    void renderModel(std::shared_ptr<platform::shader::Shader> shader, const data::ModelData& modelData,
        const math::Mat4& transform);

    void beginParticleEffect(std::shared_ptr<platform::shader::Shader> shader);
    void renderParticle();
    void endParticleEffect();

    void beginDepthCapture() {
    }

    void endDepthCapture() {
    }

    static std::shared_ptr<RendererProxy> create(renderer::lowlevel::LowLevelRenderer& lowLevelRenderer);

private:
    renderer::lowlevel::LowLevelRenderer& m_lowLevelRenderer;
    std::shared_ptr<renderer::CubemapRenderer> m_cubemapRenderer;
    std::shared_ptr<renderer::ModelRenderer> m_modelRenderer;
    std::shared_ptr<renderer::ParticleRenderer> m_particleRenderer;
};
}