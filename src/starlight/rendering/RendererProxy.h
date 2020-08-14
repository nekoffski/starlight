#pragma once

#include "starlight/asset/AssetManager.hpp"
#include "starlight/rendering/renderer/CubemapRenderer.h"
#include "starlight/rendering/renderer/ModelRenderer.h"
#include "starlight/rendering/renderer/ParticleRenderer.h"

namespace sl::rendering {

class RendererProxy {
public:
    explicit RendererProxy(std::shared_ptr<renderer::CubemapRenderer> cubemapRenderer,
        std::shared_ptr<renderer::ModelRenderer> modelRenderer,
        std::shared_ptr<renderer::ParticleRenderer> particleRenderer)
        : m_cubemapRenderer(std::move(cubemapRenderer))
        , m_modelRenderer(std::move(modelRenderer))
        , m_particleRenderer(std::move(particleRenderer)) {
    }

    void renderCubemap(const std::shared_ptr<platform::texture::Cubemap>& cubemap,
        const std::shared_ptr<platform::shader::Shader>& cubemapShader,
        const std::shared_ptr<framework::graphics::camera::Camera>& camera) {
        m_cubemapRenderer->render(cubemap, cubemapShader, camera);
    }

    void renderModels(const renderer::ShaderToModelRenderData& models, const std::shared_ptr<framework::graphics::camera::Camera>& camera) {
        m_modelRenderer->render(models, camera);
    }

    void renderParticles() {
        m_particleRenderer->render();
    }

    static std::shared_ptr<RendererProxy> create(framework::graphics::LowLevelRenderer& lowLevelRenderer, asset::AssetManager& assetManager) {
        auto cubemapRenderer = std::make_shared<renderer::CubemapRenderer>(lowLevelRenderer);
        auto modelRenderer = std::make_shared<renderer::ModelRenderer>(lowLevelRenderer);

        auto particleShader = assetManager.load<platform::shader::Shader>({ "/particle.vert", "/particle.frag" });
        auto particleRenderer = std::make_shared<renderer::ParticleRenderer>(lowLevelRenderer, std::move(particleShader));

        return std::make_shared<RendererProxy>(std::move(cubemapRenderer), std::move(modelRenderer), std::move(particleRenderer));
    }

private:
    std::shared_ptr<renderer::CubemapRenderer> m_cubemapRenderer;
    std::shared_ptr<renderer::ModelRenderer> m_modelRenderer;
    std::shared_ptr<renderer::ParticleRenderer> m_particleRenderer;
};
}