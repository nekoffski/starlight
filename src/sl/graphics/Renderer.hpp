#pragma once

#include <memory>

#include "LowLevelRenderer.h"

#include "renderer/CubemapRenderer.h"
#include "renderer/LightRenderer.h"
#include "renderer/ModelRenderer.h"
#include "renderer/ParticleEffectRenderer.h"
#include "renderer/ShadowRenderer.h"

namespace sl::graphics {

class Renderer {
public:
    explicit Renderer(std::shared_ptr<LowLevelRenderer> renderer)
        : m_modelRenderer(renderer)
        , m_shadowRenderer(renderer)
        , m_cubemapRenderer(renderer)
        , m_pfxRenderer(renderer) {
    }

    void renderCubemap(std::shared_ptr<Cubemap> cubemap, std::shared_ptr<Shader> cubemapShader,
        std::shared_ptr<camera::Camera> camera) {
        m_cubemapRenderer.render(cubemap, cubemapShader, camera);
    }

    void prepareDirectionalLights(ecs::ComponentView<scene::components::DirectionalLightComponent> lights,
        std::shared_ptr<graphics::Shader> shader) {
        m_lightRenderer.prepareDirectionalLights(lights, shader);
    }

    void preparePointsLights(ecs::ComponentView<scene::components::PointLightComponent> lights,
        ecs::ComponentView<scene::components::TransformComponent> transforms, std::shared_ptr<graphics::Shader> shader) {
        m_lightRenderer.preparePointsLights(lights, transforms, shader);
    }

    void renderModel(scene::components::RendererComponent& component, ecs::ComponentView<scene::components::MaterialComponent> materials,
        ecs::ComponentView<scene::components::ModelComponent> models, ecs::ComponentView<scene::components::TransformComponent> transforms,
        std::shared_ptr<graphics::camera::Camera> camera) {
        m_modelRenderer.render(component, materials, models, transforms, camera);
    }

    void renderModel(scene::components::RendererComponent& component, ecs::ComponentView<scene::components::MaterialComponent> materials,
        ecs::ComponentView<scene::components::ModelComponent> models, ecs::ComponentView<scene::components::TransformComponent> transforms,
        std::shared_ptr<graphics::camera::Camera> camera, std::shared_ptr<graphics::Shader> shader) {
        m_modelRenderer.render(component, materials, models, transforms, camera, shader);
    }

    void renderParticleEffects(ecs::ComponentView<scene::components::ParticleEffectComponent> pfxs,
        ecs::ComponentView<scene::components::TransformComponent> transforms, std::shared_ptr<graphics::camera::Camera> camera) {
        m_pfxRenderer.renderParticleEffects(pfxs, transforms, camera);
    }

    void beginDepthCapture() {
        m_shadowRenderer.beginDepthCapture();
    }
    void endDepthCapture() {
        m_shadowRenderer.endDepthCapture();
    }

    void setShadowMap(std::shared_ptr<sl::graphics::Texture> shadowMap) {
        m_shadowRenderer.setShadowMap(shadowMap);
    }

    std::shared_ptr<graphics::Shader> getDepthShader() {
        return m_shadowRenderer.getDepthShader();
    }

private:
    renderer::LightRenderer m_lightRenderer;
    renderer::ModelRenderer m_modelRenderer;
    renderer::ShadowRenderer m_shadowRenderer;
    renderer::CubemapRenderer m_cubemapRenderer;
    renderer::ParticleEffectRenderer m_pfxRenderer;
};
}
