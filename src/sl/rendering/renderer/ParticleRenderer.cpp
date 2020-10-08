#include "ParticleRenderer.h"

#include <memory>

#include "lowlevel/LowLevelRenderer.h"
#include "sl/core/log/Logger.h"
#include "sl/geometry/Geometry.hpp"
#include "sl/platform/gpu/ElementBuffer.h"
#include "sl/platform/gpu/VertexArray.h"
#include "sl/platform/gpu/VertexBuffer.h"
#include "sl/platform/shader/Shader.h"

namespace sl::rendering::renderer {

ParticleRenderer::ParticleRenderer(lowlevel::LowLevelRenderer& lowLevelRenderer)
    : Renderer(lowLevelRenderer) {
    m_vao = geometry::Geometry::getSquareVAO();
}

void ParticleRenderer::begin(std::shared_ptr<platform::shader::Shader> shader) {
    shader->setUniform("projection", m_lowLevelRenderer.getProjectionMatrix());
    m_vao->bind();
}

void ParticleRenderer::render() {
    m_lowLevelRenderer.renderVertexArray(m_vao);
}

void ParticleRenderer::end() {
    m_vao->unbind();
}
}
