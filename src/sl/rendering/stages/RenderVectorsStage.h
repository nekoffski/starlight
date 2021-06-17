#pragma once

#include "sl/rendering/DefaultFrameBufferRenderPass.h"

#include "sl/gfx/buffer/VertexArray.h"
#include "sl/physx/Vector.h"
#include "sl/utils/Globals.h"

namespace sl::rendering::stages {

class RenderVectorsStage : public DefaultFrameBufferRenderPass::Stage {
public:
    explicit RenderVectorsStage()
        : m_lineVertexArray(GLOBALS().geom->lineVAO)
        , m_lineShader(GLOBALS().shaders->singleColorShader) {
    }

    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene) override {
        m_lineShader->enable();
        m_lineShader->setUniform("projection", scene.camera->getProjectionMatrix());
        m_lineShader->setUniform("view", scene.camera->getViewMatrix());

        m_lineVertexArray->bind();

        // TODO: refactor!

        for (const auto& [vector, color] : scene.vectors) {
            m_lineShader->setUniform("color", color);

            auto scale = math::length(vector.direction) * 0.75f;
            auto normalizedDirection = math::normalize(vector.direction);

            math::Mat4 rotationMatrix = {
                normalizedDirection.x, 0.0f, 0.0f, 0.0f,
                0.0f, normalizedDirection.y, 0.0f, 0.0f,
                0.0f, 0.0f, normalizedDirection.z, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };

            auto model = math::translate(vector.origin) * math::scale(scale) * rotationMatrix;
            m_lineShader->setUniform("model", model);

            renderer.renderLine();
        }

        m_lineVertexArray->unbind();

        m_lineShader->disable();
    }

private:
    std::shared_ptr<gfx::buffer::VertexArray> m_lineVertexArray;
    std::shared_ptr<gfx::Shader> m_lineShader;
};

}