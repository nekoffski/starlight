#pragma once

#include "sl/rendering/DefaultFrameBufferRenderPass.h"

#include "sl/gfx/buffer/VertexArray.h"

namespace sl::rendering::stages {

class RenderSkyboxStage : public DefaultFrameBufferRenderPass::Stage {
public:
    explicit RenderSkyboxStage();

    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene) override;

private:
    std::shared_ptr<gfx::buffer::VertexArray> m_cubemapVertexArray;
};

}