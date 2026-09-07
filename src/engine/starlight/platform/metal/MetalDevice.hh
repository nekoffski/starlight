#pragma once

#include <memory>
#include <vector>

#include "Metal.hh"
#include "MetalRenderFrameFence.hh"
#include "MetalRenderFrameRecorder.hh"
#include "MetalResourcePool.hh"
#include "starlight/core/Config.hh"
#include "starlight/renderer/rhi/RenderDevice.hh"

namespace sl {

class MetalDevice : public RenderDevice {
    struct Guard {};

   public:
    explicit MetalDevice(const Config& config, Guard guard);
    ~MetalDevice() override;

    static std::unique_ptr<MetalDevice> create(const Config& config);

   private:
    Result<SurfaceHandle> attachSurface(
        std::shared_ptr<RenderSurfaceProvider> provider
    ) override {
        return m_resourcePool.attachSurface(std::move(provider));
    }

    void destroySurface(SurfaceHandle) override {
        // noop for metal, no renderer involved in creating surface
    }

    Result<void> trySubmitFrame(RecordFrame callback) override;

    Config m_config;
    MetalContext m_ctx;
    MetalResourcePool m_resourcePool;

    std::vector<std::unique_ptr<MetalRenderFrameFence>> m_frames;
    u8 m_nextFrameSlot{0u};
};

}  // namespace sl
