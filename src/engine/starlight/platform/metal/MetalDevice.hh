#pragma once

#include <memory>

#include "Metal.hh"
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
    Config m_config;

    MTL::Device* m_device{nullptr};
    MTL::CommandQueue* m_queue{nullptr};
};

}  // namespace sl
