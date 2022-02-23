#pragma once

#include "sl/gfx/VertexArray.h"
#include "sl/rendering/DefaultFrameBufferRenderPass.h"

namespace sl::rendering::stages {

class RenderSkyboxStage : public Stage {
   public:
    explicit RenderSkyboxStage();

    void execute(gfx::Renderer& renderer, scene::Scene& scene, gfx::FrameBuffer*) override;

   private:
    std::shared_ptr<gfx::VertexArray> m_cubemapVertexArray;
};

}  // namespace sl::rendering::stages