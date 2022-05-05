#pragma once

#include "sl/gpu/VertexArray.h"
#include "sl/rendering/DefaultFrameBufferRenderPass.h"

namespace sl::rendering::stages {

class RenderSkyboxStage : public Stage {
   public:
    explicit RenderSkyboxStage();

    void execute(gpu::Renderer& renderer, scene::Scene& scene, gpu::FrameBuffer*) override;

   private:
    std::shared_ptr<gpu::VertexArray> m_cubemapVertexArray;
};

}  // namespace sl::rendering::stages