#include "Platform.hh"

#include "starlight/core/Log.hh"

#ifdef SL_PLATFORM_DARWIN
#include "metal/MetalDevice.hh"
#endif

namespace sl {

void Platform::logInfo() {
#ifdef SL_PLATFORM_DARWIN
    log::info("detected platform: macOS");
#endif
}

std::unique_ptr<RenderDevice> Platform::createRenderDevice(const Config& cfg) {
#ifdef SL_PLATFORM_DARWIN
    log::trace("selected RenderDevice implementation - Metal");
    return MetalDevice::create(cfg);
#endif
}

}  // namespace sl