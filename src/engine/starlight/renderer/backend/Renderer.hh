#pragma once

#include "RenderOutput.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/renderer/rhi/RenderDevice.hh"
#include "starlight/renderer/rhi/RenderSurfaceProvider.hh"

namespace sl {

class Renderer : public NonCopyable, public NonMovable {
   public:
    explicit Renderer(const Config& config, RenderDevice& device);

    bool hasPendingWork() const;
    void tick();
    void flush();

    Result<RenderOutput> createOutput(
        std::shared_ptr<RenderSurfaceProvider> surfaceProvider
    );
    void destroyOutput(RenderOutput output);

   private:
    Config m_config;
    RenderDevice& m_device;
};

}  // namespace sl
