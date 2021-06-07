#pragma once

#include <memory>

#include "sl/core/Logger.h"
#include "sl/gfx/LowLevelRenderer.h"
#include "sl/gfx/buffer/VertexArray.h"
#include "sl/gfx/camera/Camera.h"
#include "sl/physx/BoundingBox.h"
#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/utils/Globals.h"

namespace sl::gfx::renderer {

using namespace scene::components;

class BoundingBoxRenderer {
public:
    explicit BoundingBoxRenderer(LowLevelRenderer& renderer)
        : m_renderer(renderer)
        , m_boundingBoxShader(gfx::Shader::load(
              GLOBALS().config.paths.shaders + "/rigid_body.vert", GLOBALS().config.paths.shaders + "/rigid_body.frag")) {
    }

    void renderBoundingBoxes(ecs::ComponentView<RigidBodyComponent> rigidBodies, ecs::ComponentView<TransformComponent> transforms, gfx::camera::Camera& camera) {
        m_boundingBoxShader->enable();
        m_boundingBoxShader->setUniform("projection", camera.getProjectionMatrix());
        m_boundingBoxShader->setUniform("view", camera.getViewMatrix());

        auto settings = m_renderer.getSettings();
        settings.polygonMode = STARL_LINE;

        m_renderer.setTemporarySettings(settings);

        for (auto& rigidyBody : rigidBodies) {
            auto& entityId = rigidyBody.ownerEntityId;

            auto modelMatrix =
                transforms.doesEntityOwnComponent(entityId) ? transforms.getByEntityId(entityId).transformation : sl::math::identityMatrix;

            m_boundingBoxShader->setUniform("model", modelMatrix);

            if (not rigidyBody.renderBoundingBox || rigidyBody.boundingBox == nullptr)
                continue;

            renderBoundingBox(*rigidyBody.boundingBox);
        }

        m_boundingBoxShader->disable();
        m_renderer.restoreSettings();
    }

private:
    void renderBoundingBox(physx::BoundingBox& boundingBox) {
        auto vao = boundingBox.getVertexArray();

        vao->bind();
        m_renderer.renderVertexArray(vao);
        vao->unbind();
    }

    std::shared_ptr<gfx::Shader> m_boundingBoxShader;
    LowLevelRenderer& m_renderer;
};
}
