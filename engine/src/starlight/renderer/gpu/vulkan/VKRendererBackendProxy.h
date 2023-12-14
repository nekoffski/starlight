#pragma once

#include "fwd.h"

#include "starlight/renderer/RendererBackendProxy.h"
#include "VKCommandBuffer.h"

namespace sl::vk {

class VKRendererBackendProxy : public RendererBackendProxy {
public:
    explicit VKRendererBackendProxy(VKRendererBackend* backend);

    void drawGeometry(const GeometryRenderData& renderData) override;
    VKCommandBuffer* getCommandBuffer() override;
    u32 getImageIndex() override;

private:
    VKRendererBackend* m_backend;
};
}  // namespace sl::vk
