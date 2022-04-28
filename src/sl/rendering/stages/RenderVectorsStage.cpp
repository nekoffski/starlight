#include "RenderVectorsStage.h"

#include <kc/core/Profiler.h>
#include <kc/math/Format.h>

#include "sl/glob/Globals.h"

namespace sl::rendering::stages {

RenderVectorsStage::RenderVectorsStage()
    : m_lineVertexArray(glob::Globals::get().geom->lineVAO)
    , m_lineShader(glob::Globals::get().shaders->singleColorShader) {}

void RenderVectorsStage::execute(gfx::Renderer& renderer, scene::Scene& scene, gfx::FrameBuffer*) {
    PROFILE_FUNCTION();

    setCameraUniforms(*scene.camera);

    m_lineVertexArray->bind();
    m_lineShader->enable();

    for (const auto& [vector, color] : scene.vectors) renderVector(vector, color, renderer);

    m_lineVertexArray->unbind();
    m_lineShader->disable();

    scene.vectors.clear();
}

void RenderVectorsStage::renderVector(
    const physx::Vector& vector, const glm::vec3& color, gfx::Renderer& renderer
) {
    LOG_TRACE("Rendering: {}/{}", vector, color);
    setVectorUniforms(vector, color);
    renderer.renderLine();
}

math::Mat4 RenderVectorsStage::calculateModelMatrix(const physx::Vector& vector) {
    static constexpr float scaleFactor = 0.75f;

    const auto scale               = math::length(vector.direction) * scaleFactor;
    const auto normalizedDirection = math::normalize(vector.direction);

    const auto& x = normalizedDirection.x;
    const auto& y = normalizedDirection.y;
    const auto& z = normalizedDirection.z;

    // clang-format off
    math::Mat4 orientationMatrix = {
        x,    0.0f, 0.0f, 0.0f,
        0.0f, y,    0.0f, 0.0f,
        0.0f, 0.0f, z,    0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    // clang-format on

    return math::translate(vector.origin) * math::scale(scale) * orientationMatrix;
}

void RenderVectorsStage::setVectorUniforms(const physx::Vector& vector, const glm::vec3& color) {
    m_lineShader->setUniform("color", color);
    m_lineShader->setUniform("modelMatrix", calculateModelMatrix(vector));
}

void RenderVectorsStage::setCameraUniforms(cam::Camera& camera) {
    m_lineShader->setUniform("projectionMatrix", camera.getProjectionMatrix());
    m_lineShader->setUniform("viewMatrix", camera.getViewMatrix());
}
}  // namespace sl::rendering::stages