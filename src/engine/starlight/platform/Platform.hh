#pragma once

#include "starlight/core/Concepts.hh"
#include "starlight/core/Config.hh"
#include "starlight/renderer/rhi/RenderDevice.hh"

#ifdef _WIN32
#define SL_PLATFORM_WINDOWS
#elif __APPLE__
#define SL_PLATFORM_DARWIN
#else
#define SL_PLATFORM_LINUX
#endif

namespace sl {

class Platform : public StaticClass {
   public:
    static void logInfo();

    static std::unique_ptr<RenderDevice> createRenderDevice(const Config& cfg);
};

}  // namespace sl