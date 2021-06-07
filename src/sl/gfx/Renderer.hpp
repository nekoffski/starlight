#pragma once

#include <memory>

#include "LowLevelRenderer.h"

#include "renderer/BoundingBoxRenderer.h"
#include "renderer/CubemapRenderer.h"
#include "renderer/LightRenderer.h"
#include "renderer/MeshRenderer.h"
#include "renderer/ParticleEffectRenderer.h"
#include "renderer/ShadowRenderer.h"
#include "renderer/VectorRenderer.h"
#include "sl/utils/Globals.h"

namespace sl::gfx {

using VectorRenderData = renderer::VectorRenderer::VectorData;

class Renderer {
public:
    explicit Renderer(LowLevelRenderer& renderer)
        : m_meshRenderer(renderer)
        , m_shadowRenderer(renderer)
        , m_cubemapRenderer(renderer)
        , m_pfxRenderer(renderer)
        , m_boundingBoxRenderer(renderer)
        , m_vectorRenderer(renderer) {
    }

    void renderVectors(const std::vector<VectorRenderData>& vectors, camera::Camera& camera,
        gfx::Shader& shader = *GLOBALS().shaders->singleColorShader) {

        m_vectorRenderer.renderVectors(vectors, camera, shader);
    }
    void renderBoundingBoxes(ecs::ComponentView<scene::components::RigidBodyComponent> rigidBodies,

        ecs::ComponentView<scene::components::TransformComponent> transforms, gfx::camera::Camera& camera) {
        m_boundingBoxRenderer.renderBoundingBoxes(std::move(rigidBodies), std::move(transforms), camera);
    }

    void renderCubemap(Cubemap& cubemap, Shader& cubemapShader, camera::Camera& camera) {
        m_cubemapRenderer.render(cubemap, cubemapShader, camera);
    }

    void prepareDirectionalLights(ecs::ComponentView<scene::components::DirectionalLightComponent> lights,
        gfx::Shader& shader) {
        m_lightRenderer.prepareDirectionalLights(lights, shader);
    }

    void preparePointsLights(ecs::ComponentView<scene::components::PointLightComponent> lights,
        ecs::ComponentView<scene::components::TransformComponent> transforms, gfx::Shader& shader) {
        m_lightRenderer.preparePointsLights(lights, transforms, shader);
    }

    void renderModel(scene::components::MeshRendererComponent& component, ecs::ComponentView<scene::components::MaterialComponent> materials,
        ecs::ComponentView<scene::components::ModelComponent> models, ecs::ComponentView<scene::components::TransformComponent> transforms,
        gfx::camera::Camera& camera) {
        m_meshRenderer.render(component, materials, models, transforms, camera);
    }

    void renderModel(scene::components::MeshRendererComponent& component, ecs::ComponentView<scene::components::MaterialComponent> materials,
        ecs::ComponentView<scene::components::ModelComponent> models, ecs::ComponentView<scene::components::TransformComponent> transforms,
        gfx::camera::Camera& camera, gfx::Shader& shader) {
        m_meshRenderer.render(component, materials, models, transforms, camera, shader);
    }

    void renderParticleEffects(ecs::ComponentView<scene::components::ParticleEffectComponent> pfxs,
        ecs::ComponentView<scene::components::TransformComponent> transforms, gfx::camera::Camera& camera) {
        m_pfxRenderer.renderParticleEffects(pfxs, transforms, camera);
    }

    void beginDepthCapture() {
        m_shadowRenderer.beginDepthCapture();
    }
    void endDepthCapture() {
        m_shadowRenderer.endDepthCapture();
    }

    void setShadowMap(std::shared_ptr<sl::gfx::Texture> shadowMap) {
        m_shadowRenderer.setShadowMap(shadowMap);
    }

    std::shared_ptr<gfx::Shader> getDepthShader() {
        return m_shadowRenderer.getDepthShader();
    }

private:
    renderer::LightRenderer m_lightRenderer;
    renderer::MeshRenderer m_meshRenderer;
    renderer::ShadowRenderer m_shadowRenderer;
    renderer::CubemapRenderer m_cubemapRenderer;
    renderer::ParticleEffectRenderer m_pfxRenderer;
    renderer::BoundingBoxRenderer m_boundingBoxRenderer;
    renderer::VectorRenderer m_vectorRenderer;
};
}
