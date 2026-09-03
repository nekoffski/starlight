#include "MetalDevice.hh"

namespace sl {

MetalDevice::MetalDevice(const Config& config, Guard guard)
    : m_config(config), m_resourcePool(config, m_ctx) {
    log::trace("MetalDevice created");
}

MetalDevice::~MetalDevice() { log::trace("MetalDevice destroyed"); }

std::unique_ptr<MetalDevice> MetalDevice::create(const Config& config) {
    return std::make_unique<MetalDevice>(config, Guard{});
}

}  // namespace sl
