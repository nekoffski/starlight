#pragma once

#include "sl/rendering/CustomFrameBufferRenderPass.h"

#include "sl/gfx/Shader.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/buffer/VertexArray.h"

#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/TransformComponent.h"

#include "sl/rendering/utils/Mesh.h"
#include "sl/rendering/utils/Misc.h"

namespace sl::rendering::stages {

const float shadowWidth = gfx::Texture::shadowMapSize;
const float shadowHeight = shadowWidth;

const float aspect = shadowWidth / shadowHeight;

const float near = 1.0f;
const float far = 25.0f;

class CapturePointDepthMapsStage : public CustomFrameBufferRenderPass::Stage {
public:
    explicit CapturePointDepthMapsStage()
        : m_depthShader(gfx::Shader::load(
              GLOBALS().config.paths.shaders + "/point_depth_capture.vert", GLOBALS().config.paths.shaders + "/point_depth_capture.frag",
              GLOBALS().config.paths.shaders + "/point_depth_capture.geom")) {

        m_shadowProjection = math::perspective(math::toRadians(90.0f), aspect, near, far);
    }

    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene, gfx::buffer::FrameBuffer& frameBuffer) override {
        SL_PROFILE_FUNCTION();

        renderer.setTemporaryViewport(gfx::ViewFrustum::Viewport { gfx::Texture::shadowMapSize, gfx::Texture::shadowMapSize });

        auto settings = renderer.getSettings();
        settings.cullFace = STARL_FRONT;
        renderer.setTemporarySettings(settings);

        m_depthShader->enable();
        m_depthShader->setUniform("far_plane", far);

        using namespace sl::scene::components;

        auto [meshRenderers, transforms, pointLights, models] =
            scene.ecsRegistry.getComponentsViews<MeshRendererComponent, TransformComponent, PointLightComponent, ModelComponent>();

        for (auto& light : pointLights) {
            if (not light.isActive)
                continue;

            std::vector<math::Mat4> shadowTransforms;

            auto& transform = utils::getModelMatrix(light.ownerEntityId, transforms);
            auto lightPos = transform * light.position;

            shadowTransforms.push_back(m_shadowProjection *
                glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
            shadowTransforms.push_back(m_shadowProjection *
                glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
            shadowTransforms.push_back(m_shadowProjection *
                glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
            shadowTransforms.push_back(m_shadowProjection *
                glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
            shadowTransforms.push_back(m_shadowProjection *
                glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
            shadowTransforms.push_back(m_shadowProjection *
                glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

            for (int i = 0; i < 6; ++i)
                m_depthShader->setUniform(
                    "shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);

            m_depthShader->setUniform("lightPos", lightPos);
            frameBuffer.bindCubemap(*light.omnidirectionalShadowMap);

            for (auto& meshRenderer : meshRenderers) {
                const auto& entityId = meshRenderer.ownerEntityId;

                if (not models.doesEntityOwnComponent(entityId))
                    continue;

                utils::renderModel(renderer, *m_depthShader, models.getByEntityId(entityId),
                    utils::getModelMatrix(entityId, transforms));
            }
        }

        m_depthShader->disable();

        renderer.restoreSettings();
        renderer.restoreViewport();
    }

    std::shared_ptr<gfx::Shader> m_depthShader;

    math::Mat4 m_shadowProjection;
};

}