#pragma once

#include "data/ModelData.h"
#include "renderer/fwd.h"
#include "renderer/lowlevel/LowLevelRenderer.h"
#include "starlight/math/Matrix.hpp"
#include "starlight/platform/fwd.h"

namespace sl::rendering {

class RendererProxy {
public:
    explicit RendererProxy(std::shared_ptr<renderer::CubemapRenderer> cubemapRenderer,
        std::shared_ptr<renderer::ModelRenderer> modelRenderer,
        std::shared_ptr<renderer::ParticleRenderer> particleRenderer);

    void renderCubemap(const std::shared_ptr<platform::texture::Cubemap>& cubemap,
        const std::shared_ptr<platform::shader::Shader>& cubemapShader,
        const std::shared_ptr<rendering::camera::Camera>& camera);

    void renderModels(std::shared_ptr<platform::shader::Shader> shader, const data::ModelData& modelData,
        const math::Mat4& transform);

    void renderParticles();

    static std::shared_ptr<RendererProxy> create(renderer::lowlevel::LowLevelRenderer& lowLevelRenderer);

private:
    std::shared_ptr<renderer::CubemapRenderer> m_cubemapRenderer;
    std::shared_ptr<renderer::ModelRenderer> m_modelRenderer;
    std::shared_ptr<renderer::ParticleRenderer> m_particleRenderer;
};
}