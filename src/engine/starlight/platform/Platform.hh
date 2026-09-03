#pragma once

#include "starlight/core/Concepts.hh"
#include "starlight/core/Config.hh"
#include "starlight/renderer/rhi/RenderDevice.hh"

namespace sl {

class Platform : public StaticClass {
   public:
    static void logInfo();

    static std::unique_ptr<RenderDevice> createRenderDevice(const Config& cfg);
};

}  // namespace sl