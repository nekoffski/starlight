#pragma once

#include <memory>

#include "sl/core/Logger.h"
#include "sl/geom/ModelLoader.hpp"
#include "sl/gfx/LowLevelRenderer.h"
#include "sl/gfx/buffer/VertexArray.h"
#include "sl/gfx/camera/Camera.h"
#include "sl/physx/BoundingBox.h"
#include "sl/physx/Vector.h"
#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/utils/Globals.h"

namespace sl::gfx::renderer {

using namespace scene::components;

class VectorRenderer {

public:
    explicit VectorRenderer(std::shared_ptr<LowLevelRenderer> renderer)
        : m_renderer(renderer)
        , m_vectorShader(gfx::Shader::load(
              GLOBALS().config.paths.shaders + "/vector.vert", GLOBALS().config.paths.shaders + "/vector.frag")) {

        // TODO: fixme
        m_vectorShader->setId(16);
    }

    void renderVectors(const std::vector<physx::Vector>& vectors, camera::Camera& camera) {
        m_vectorShader->enable();
        m_vectorShader->setUniform("projection", camera.getProjectionMatrix());
        m_vectorShader->setUniform("view", camera.getViewMatrix());

        auto& vao = GLOBALS().geom->lineVAO;
        vao->bind();

        // TODO: refactor!

        for (const auto& vector : vectors) {
            auto scale = math::length(vector.direction) * 0.75f;
            auto dir = math::normalize(vector.origin);
            auto ff = math::normalize(vector.direction);

            math::Mat4 rot = {
                ff.x, 0.0f, 0.0f, 0.0f,
                0.0f, ff.y, 0.0f, 0.0f,
                0.0f, 0.0f, ff.z, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };

            auto model = math::translate(vector.origin) * math::scale(scale) * rot;
            m_vectorShader->setUniform("model", model);

            m_renderer->renderLine();
        }

        vao->unbind();

        m_vectorShader->disable();
    }

private:
    std::shared_ptr<LowLevelRenderer> m_renderer;
    std::shared_ptr<gfx::Shader> m_vectorShader;
};
}
