#pragma once

#include <memory>
#include <unordered_map>

#include "lowlevel/LowLevelRenderer.h"
#include "sl/core/log/Logger.h"
#include "sl/geometry/Geometry.hpp"
#include "sl/platform/gpu/ElementBuffer.h"
#include "sl/platform/gpu/VertexArray.h"
#include "sl/platform/gpu/VertexBuffer.h"
#include "sl/platform/shader/Shader.h"
#include "sl/rendering/renderer/Renderer.h"

namespace sl::rendering::renderer {

class ParticleRenderer : public Renderer {
public:
    explicit ParticleRenderer(lowlevel::LowLevelRenderer& lowLevelRenderer)
        : Renderer(lowLevelRenderer) {
        m_vao = geometry::Geometry::getSquareVAO();
    }

    void begin(std::shared_ptr<platform::shader::Shader> shader) {
        shader->setUniform("projection", m_lowLevelRenderer.getProjectionMatrix());
        m_lowLevelRenderer.renderVertexArray(m_vao);
    }

    void end() {
    }

private:
    std::shared_ptr<platform::gpu::VertexArray> m_vao;
};
}