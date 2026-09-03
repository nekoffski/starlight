#pragma once

#include <memory>

#include "Metal.hh"
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
    Result<SurfaceHandle> createSurface(
        std::shared_ptr<RenderSurfaceProvider> provider
    ) override {
        return m_resourcePool.createSurface(std::move(provider));
    }

    void destroySurface(SurfaceHandle handle) override {
        m_resourcePool.destroySurface(handle);
    }

    Config m_config;
    MetalContext m_ctx;
    MetalResourcePool m_resourcePool;
};

}  // namespace sl
