#pragma once

#include "sl/rendering/DefaultFrameBufferRenderPass.h"

#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/utils/Globals.h"

namespace sl::rendering::stages {

class RenderBoundingBoxesStage : public DefaultFrameBufferRenderPass::Stage {
public:
    explicit RenderBoundingBoxesStage()
        : m_boundingBoxShader(gfx::Shader::load(
              GLOBALS().config.paths.shaders + "/rigid_body.vert", GLOBALS().config.paths.shaders + "/rigid_body.frag")) {
    }

    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene) override {
        m_boundingBoxShader->enable();
        m_boundingBoxShader->setUniform("projection", scene.camera->getProjectionMatrix());
        m_boundingBoxShader->setUniform("view", scene.camera->getViewMatrix());

        auto settings = renderer.getSettings();
        settings.polygonMode = STARL_LINE;

        using namespace sl::scene::components;

        auto [rigidBodies, transforms] = scene.ecsRegistry.getComponentsViews<RigidBodyComponent, TransformComponent>();

        renderer.setTemporarySettings(settings);

        for (auto& rigidyBody : rigidBodies) {
            auto& entityId = rigidyBody.ownerEntityId;

            auto modelMatrix =
                transforms.doesEntityOwnComponent(entityId) ? transforms.getByEntityId(entityId).transformation : sl::math::identityMatrix;

            if (rigidyBody.boundingBox != nullptr) {
                physx::Vector vector {
                    modelMatrix * rigidyBody.boundingBox->getCenterOfMass(), rigidyBody.velocity
                };
                scene.vectors.emplace_back(physx::ColoredVector { std::move(vector), color::green });
            }

            m_boundingBoxShader->setUniform("model", modelMatrix);

            if (not rigidyBody.renderBoundingBox || rigidyBody.boundingBox == nullptr)
                continue;

            renderBoundingBox(renderer, *rigidyBody.boundingBox);
        }

        m_boundingBoxShader->disable();
        renderer.restoreSettings();
    }

private:
    void renderBoundingBox(gfx::LowLevelRenderer& renderer, physx::BoundingBox& boundingBox) {
        auto vao = boundingBox.getVertexArray();

        vao->bind();
        renderer.renderVertexArray(vao);
        vao->unbind();
    }

    std::shared_ptr<gfx::Shader> m_boundingBoxShader;
};

}