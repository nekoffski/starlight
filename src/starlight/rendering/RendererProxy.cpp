#include "RendererProxy.h"

#include "starlight/asset/AssetManager.hpp"
#include "starlight/rendering/renderer/CubemapRenderer.h"
#include "starlight/rendering/renderer/ModelRenderer.h"
#include "starlight/rendering/renderer/ParticleRenderer.h"

namespace sl::rendering {

RendererProxy::RendererProxy(std::shared_ptr<renderer::CubemapRenderer> cubemapRenderer,
    std::shared_ptr<renderer::ModelRenderer> modelRenderer,
    std::shared_ptr<renderer::ParticleRenderer> particleRenderer)
    : m_cubemapRenderer(std::move(cubemapRenderer))
    , m_modelRenderer(std::move(modelRenderer))
    , m_particleRenderer(std::move(particleRenderer)) {
}

void RendererProxy::renderCubemap(std::shared_ptr<platform::texture::Cubemap> cubemap,
    std::shared_ptr<platform::shader::Shader> cubemapShader,
    std::shared_ptr<rendering::camera::Camera> camera) {
    m_cubemapRenderer->render(cubemap, cubemapShader, camera);
}

void RendererProxy::renderModel(std::shared_ptr<platform::shader::Shader> shader, const data::ModelData& modelData,
    const math::Mat4& transform) {
    m_modelRenderer->render(shader, modelData, transform);
}

std::shared_ptr<RendererProxy> RendererProxy::create(renderer::lowlevel::LowLevelRenderer& lowLevelRenderer) {
    auto cubemapRenderer = std::make_shared<renderer::CubemapRenderer>(lowLevelRenderer);
    auto modelRenderer = std::make_shared<renderer::ModelRenderer>(lowLevelRenderer);
    auto particleRenderer = std::make_shared<renderer::ParticleRenderer>(lowLevelRenderer);

    return std::make_shared<RendererProxy>(std::move(cubemapRenderer), std::move(modelRenderer), std::move(particleRenderer));
}
}