#pragma once

#include "sl/rendering/DefaultFrameBufferRenderPass.h"

#include "sl/gfx/VertexArray.h"

namespace sl::rendering::stages {

class RenderSkyboxStage : public Stage {
public:
    explicit RenderSkyboxStage();

    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene, gfx::FrameBuffer*) override;

private:
    std::shared_ptr<gfx::VertexArray> m_cubemapVertexArray;
};

}