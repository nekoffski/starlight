#pragma once

#include "sl/rendering/CustomFrameBufferRenderPass.h"

#include "sl/gfx/Shader.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/buffer/VertexArray.h"
#include "sl/utils/Globals.h"

#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/TransformComponent.h"

#include "sl/rendering/utils/Mesh.h"

namespace sl::rendering::stages {

using namespace sl::scene::components;

class CaptureDirectionalDepthMapsStage : public CustomFrameBufferRenderPass::Stage {
public:
    explicit CaptureDirectionalDepthMapsStage()
        : m_depthShader(gfx::Shader::load(
              GLOBALS().config.paths.shaders + "/depth_capture.vert", GLOBALS().config.paths.shaders + "/depth_capture.frag")) {
    }

    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene, gfx::buffer::FrameBuffer& frameBuffer) override {
        renderer.setTemporaryViewport(gfx::ViewFrustum::Viewport { gfx::Texture::shadowMapSize, gfx::Texture::shadowMapSize });

        m_depthShader->enable();

        auto settings = renderer.getSettings();
        settings.cullFace = STARL_FRONT;
        renderer.setTemporarySettings(settings);

        auto [rendererComponents, transforms, directionalLights, models] =
            scene.ecsRegistry.getComponentsViews<MeshRendererComponent, TransformComponent, DirectionalLightComponent, ModelComponent>();

        for (auto& directionalLight : directionalLights) {
            if (not directionalLight.isActive)
                continue;

            const math::Vec3 sceneOrigin = { 0.0f, 0.0f, 0.0f };

            physx::Vector vector {
                sceneOrigin, directionalLight.direction * 1000.0f
            };
            scene.vectors.emplace_back(physx::ColoredVector { std::move(vector), color::blue });

            frameBuffer.bindTexture(*directionalLight.shadowMap);
            m_depthShader->setUniform("lightSpaceMatrix", directionalLight.spaceMatrix);

            for (auto& rendererComponent : rendererComponents) {
                auto& entityId = rendererComponent.ownerEntityId;
                auto& model = models.getByEntityId(entityId);

                auto transformMatrix = transforms.doesEntityOwnComponent(entityId) ? transforms.getByEntityId(entityId).transformation
                                                                                   : math::identityMatrix;

                utils::renderModel(renderer, *m_depthShader, model, transformMatrix);
            }
        }

        m_depthShader->disable();

        renderer.restoreSettings();
        renderer.restoreViewport();
    }

private:
    std::shared_ptr<gfx::Shader> m_depthShader;
};

}