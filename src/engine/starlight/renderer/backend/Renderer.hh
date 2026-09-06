#pragma once

#include <queue>

#include "RenderOutput.hh"
#include "RenderRequest.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/renderer/rhi/RenderDevice.hh"
#include "starlight/renderer/rhi/RenderSurfaceProvider.hh"

namespace sl {

class Renderer : public NonCopyable, public NonMovable {
   public:
    explicit Renderer(const Config& config, RenderDevice& device);

    bool tick();
    void flush();

    Result<void> submit(const RenderRequest& request);

    Result<RenderOutput> createOutput(
        std::shared_ptr<RenderSurfaceProvider> surfaceProvider
    );
    void destroyOutput(RenderOutput output);

   private:
    u64 frameIndex() const;

    Config m_config;
    RenderDevice& m_device;

    std::queue<RenderRequest> m_pendingRequests;
    u64 m_frameNumber;
};

}  // namespace sl
