#pragma once

#include "sl/rendering/DefaultFrameBufferRenderPass.h"

#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/TransformComponent.h"

#include "sl/core/Profiler.h"
#include "sl/gfx/LowLevelRenderer.h"
#include "sl/rendering/utils/Mesh.h"
#include "sl/utils/Globals.h"

namespace sl::rendering::stages {

void setMaterial(const scene::components::MaterialComponent& material, gfx::Shader& shader) {
    shader.setUniform("material.ambientColor", material.ambientColor);
    shader.setUniform("material.diffuseColor", material.diffuseColor);
    shader.setUniform("material.specularColor", material.specularColor);
    shader.setUniform("material.shininess", material.shininess);
}

using namespace sl::scene::components;

const MaterialComponent DEFAULT_MATERIAL = scene::components::MaterialComponent();

class RenderMeshesStage : public DefaultFrameBufferRenderPass::Stage {
public:
    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene) override {
        auto [rendererComponents, transforms, models, materials] =
            scene.ecsRegistry.getComponentsViews<MeshRendererComponent, TransformComponent, ModelComponent, MaterialComponent>();

        for (auto& component : rendererComponents) {
            auto& entityId = component.ownerEntityId;
            auto& shader = *component.shader;
            auto& camera = *scene.camera;

            shader.enable();

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

            auto settings = renderer.getSettings();
            settings.polygonMode = component.polygonMode;
            renderer.setTemporarySettings(settings);

            float t = CLOCK().nowAsFloat();
            shader.setUniform("t", t);
            shader.setUniform("projection", camera.getProjectionMatrix());

            utils::renderModel(renderer, shader, model, transformMatrix);

            renderer.restoreSettings();

            shader.disable();
        }
    }
};

}