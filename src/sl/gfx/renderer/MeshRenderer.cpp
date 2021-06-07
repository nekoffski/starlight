#include "MeshRenderer.h"

#include "sl/core/Profiler.h"
#include "sl/gfx/LowLevelRenderer.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/utils/Globals.h"

namespace sl::gfx::renderer {

const scene::components::MaterialComponent DEFAULT_MATERIAL = scene::components::MaterialComponent();

MeshRenderer::MeshRenderer(LowLevelRenderer& renderer)
    : m_renderer(renderer) {
}

void MeshRenderer::render(scene::components::MeshRendererComponent& component, ecs::ComponentView<scene::components::MaterialComponent> materials,
    ecs::ComponentView<scene::components::ModelComponent> models, ecs::ComponentView<scene::components::TransformComponent> transforms,
    gfx::camera::Camera& camera, gfx::Shader& shader) {

    auto& entityId = component.ownerEntityId;

    if (not models.doesEntityOwnComponent(entityId))
        return;

    auto& model = models.getByEntityId(entityId);

    if (not model.isActive || not component.isActive)
        return;

    shader.setUniform("view", camera.getViewMatrix());
    shader.setUniform("viewPos", camera.getPosition());

    // TODO: POSSIBLE BRANCHING
    auto& material = materials.doesEntityOwnComponent(entityId) ? materials.getByEntityId(entityId) : DEFAULT_MATERIAL;

    setMaterial(material, shader);

    auto transformMatrix = transforms.doesEntityOwnComponent(entityId) ? transforms.getByEntityId(entityId).transformation
                                                                       : math::identityMatrix;

    auto settings = m_renderer.getSettings();
    settings.polygonMode = component.polygonMode;
    m_renderer.setTemporarySettings(settings);

    renderModel(shader, model, transformMatrix, camera);

    m_renderer.restoreSettings();
}

void MeshRenderer::render(scene::components::MeshRendererComponent& component, ecs::ComponentView<scene::components::MaterialComponent> materials,
    ecs::ComponentView<scene::components::ModelComponent> models, ecs::ComponentView<scene::components::TransformComponent> transforms,
    gfx::camera::Camera& camera) {

    render(component, materials, models, transforms, camera, *component.shader);
}

void MeshRenderer::setMaterial(const scene::components::MaterialComponent& material, gfx::Shader& shader) {
    shader.setUniform("material.ambientColor", material.ambientColor);
    shader.setUniform("material.diffuseColor", material.diffuseColor);
    shader.setUniform("material.specularColor", material.specularColor);
    shader.setUniform("material.shininess", material.shininess);
}

void MeshRenderer::renderModel(gfx::Shader& shader, scene::components::ModelComponent& model,
    const math::Mat4& transform, gfx::camera::Camera& camera) {

    float t = CLOCK().now()->value();
    shader.setUniform("t", t);
    shader.setUniform("projection", camera.getProjectionMatrix());

    for (auto& position : model.instances) {
        shader.setUniform("model", transform * math::translate(position));

        for (auto& mesh : model.meshes) {
            shader.setUniform("textures", static_cast<unsigned int>(mesh->textures.size()));
            renderMesh(*mesh);
        }
    }
}

void MeshRenderer::renderMesh(geom::Mesh& mesh) {
    int i = 0;
    for (const auto& texture : mesh.textures)
        texture->bind(i++);

    auto& vao = mesh.vertexArray;

    vao->bind();
    m_renderer.renderVertexArray(vao);
    vao->unbind();

    for (const auto& texture : mesh.textures)
        texture->unbind();
}
}
