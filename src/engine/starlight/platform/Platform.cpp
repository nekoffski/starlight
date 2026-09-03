#include "Platform.hh"

#include "starlight/core/Log.hh"

#if defined(SL_USE_METAL)
#include "metal/MetalDevice.hh"
#else
#error "Unsupported platform"
#endif

namespace sl {

void Platform::logInfo() {
#if defined(SL_PLATFORM_DARWIN)
    log::info("detected platform: macOS");
#else
#error "Unsupported platform"
#endif
}

std::unique_ptr<RenderDevice> Platform::createRenderDevice(const Config& cfg) {
#if defined(SL_USE_METAL)
    log::trace("selected RenderDevice implementation - Metal");
    return MetalDevice::create(cfg);
#else
#error "Unsupported platform"
#endif
}

}  // namespace sl
