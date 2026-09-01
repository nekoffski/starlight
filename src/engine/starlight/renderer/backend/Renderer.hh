#pragma once

#include "starlight/core/Concepts.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/renderer/rhi/RenderDevice.hh"

namespace sl {

class Renderer : public NonCopyable, public NonMovable {
   public:
    explicit Renderer(const Config& config, RenderDevice& device);

   private:
    Config m_config;
    RenderDevice& m_device;
};

}  // namespace sl
