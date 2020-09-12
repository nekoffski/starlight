#pragma once

#include <memory>
#include <unordered_map>

#include "lowlevel/LowLevelRenderer.h"
#include "starlight/core/log/Logger.h"
#include "starlight/geometry/Geometry.hpp"
#include "starlight/platform/gpu/ElementBuffer.h"
#include "starlight/platform/gpu/VertexArray.h"
#include "starlight/platform/gpu/VertexBuffer.h"
#include "starlight/platform/shader/Shader.h"
#include "starlight/rendering/renderer/Renderer.h"

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