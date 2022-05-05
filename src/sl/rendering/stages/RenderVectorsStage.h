#pragma once

#include "sl/gpu/VertexArray.h"
#include "sl/physx/Vector.h"
#include "sl/rendering/DefaultFrameBufferRenderPass.h"

namespace sl::rendering::stages {

class RenderVectorsStage : public Stage {
   public:
    explicit RenderVectorsStage();

    void execute(gpu::Renderer& renderer, scene::Scene& scene, gpu::FrameBuffer*) override;

   private:
    void renderVector(const physx::Vector& vector, const glm::vec3& color, gpu::Renderer& renderer);

    math::Mat4 calculateModelMatrix(const physx::Vector& vector);

    void setVectorUniforms(const physx::Vector& vector, const glm::vec3& color);
    void setCameraUniforms(cam::Camera& camera);

    std::shared_ptr<gpu::VertexArray> m_lineVertexArray;
    std::shared_ptr<gpu::Shader> m_lineShader;
};

}  // namespace sl::rendering::stages