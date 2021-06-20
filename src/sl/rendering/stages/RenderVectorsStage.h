#pragma once

#include "sl/rendering/DefaultFrameBufferRenderPass.h"

#include "sl/gfx/buffer/VertexArray.h"
#include "sl/physx/Vector.h"

namespace sl::rendering::stages {

class RenderVectorsStage : public DefaultFrameBufferRenderPass::Stage {
public:
    explicit RenderVectorsStage();

    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene) override;

private:
    void renderVector(const physx::ColoredVector& coloredVector, gfx::LowLevelRenderer& renderer);

    math::Mat4 calculateModelMatrix(const physx::Vector& vector);

    void setVectorUniforms(const physx::ColoredVector& coloredVector);
    void setCameraUniforms(gfx::camera::Camera& camera);

    std::shared_ptr<gfx::buffer::VertexArray> m_lineVertexArray;
    std::shared_ptr<gfx::Shader> m_lineShader;
};

}