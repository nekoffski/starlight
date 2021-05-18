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
    struct VectorData {
        physx::Vector vector;
        math::Vec3 color;
    };

    explicit VectorRenderer(std::shared_ptr<LowLevelRenderer> renderer)
        : m_renderer(renderer) {
    }

    void renderVectors(const std::vector<VectorData>& vectors, camera::Camera& camera, gfx::Shader& shader) {
        shader.enable();
        shader.setUniform("projection", camera.getProjectionMatrix());
        shader.setUniform("view", camera.getViewMatrix());

        auto& vao = GLOBALS().geom->lineVAO;
        vao->bind();

        // TODO: refactor!

        for (const auto& [vector, color] : vectors) {
            shader.setUniform("color", color);

            auto scale = math::length(vector.direction) * 0.75f;
            auto normalizedDirection = math::normalize(vector.direction);

            math::Mat4 rotationMatrix = {
                normalizedDirection.x, 0.0f, 0.0f, 0.0f,
                0.0f, normalizedDirection.y, 0.0f, 0.0f,
                0.0f, 0.0f, normalizedDirection.z, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };

            auto model = math::translate(vector.origin) * math::scale(scale) * rotationMatrix;
            shader.setUniform("model", model);

            m_renderer->renderLine();
        }

        vao->unbind();

        shader.disable();
    }

private:
    std::shared_ptr<LowLevelRenderer> m_renderer;
};

}
