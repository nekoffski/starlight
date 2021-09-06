#include "RenderVectorsStage.h"

#include "sl/core/Profiler.h"
#include "sl/utils/Globals.h"

namespace sl::rendering::stages {

RenderVectorsStage::RenderVectorsStage()
    : m_lineVertexArray(GLOBALS().geom->lineVAO)
    , m_lineShader(GLOBALS().shaders->singleColorShader) {
}

void RenderVectorsStage::execute(gfx::Renderer& renderer, scene::Scene& scene, gfx::FrameBuffer*) {
    LOG_PROFILE_FUNCTION();

    setCameraUniforms(*scene.camera);

    m_lineVertexArray->bind();
    m_lineShader->enable();

    for (const auto& coloredVector : scene.vectors)
        renderVector(coloredVector, renderer);

    m_lineVertexArray->unbind();
    m_lineShader->disable();
}

void RenderVectorsStage::renderVector(const physx::ColoredVector& coloredVector, gfx::Renderer& renderer) {
    setVectorUniforms(coloredVector);
    renderer.renderLine();
}

math::Mat4 RenderVectorsStage::calculateModelMatrix(const physx::Vector& vector) {
    static constexpr float scaleFactor = 0.75f;

    const auto scale = math::length(vector.direction) * scaleFactor;
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

void RenderVectorsStage::setVectorUniforms(const physx::ColoredVector& coloredVector) {
    const auto& [vector, color] = coloredVector;

    m_lineShader->setUniform("color", color);
    m_lineShader->setUniform("modelMatrix", calculateModelMatrix(vector));
}

void RenderVectorsStage::setCameraUniforms(gfx::camera::Camera& camera) {
    m_lineShader->setUniform("projectionMatrix", camera.getProjectionMatrix());
    m_lineShader->setUniform("viewMatrix", camera.getViewMatrix());
}
}