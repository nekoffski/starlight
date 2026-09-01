#include "MetalDevice.hh"

namespace sl {

namespace {

void logDeviceInfo(const MTL::Device& device);

}

MetalDevice::MetalDevice(const Config& config, Guard guard) : m_config(config) {
    m_device = MTL::CreateSystemDefaultDevice();
    log::expect(m_device != nullptr, "Failed to create Metal device");

    m_queue = m_device->newCommandQueue();
    log::expect(m_queue != nullptr, "Failed to create Metal command queue");

    logDeviceInfo(*m_device);
    log::trace("MetalDevice created");
}

MetalDevice::~MetalDevice() {
    m_queue->release();
    m_device->release();

    log::trace("MetalDevice destroyed");
}

std::unique_ptr<MetalDevice> MetalDevice::create(const Config& config) {
    return std::make_unique<MetalDevice>(config, Guard{});
}

namespace {

void logDeviceInfo(const MTL::Device& device) {
    const auto threads = device.maxThreadsPerThreadgroup();

    log::info(
        "selected Metal device: '{}', registry ID: {}, unified memory: {}, max "
        "buffer length: {}, max threads: {} x {} x {}",
        device.name()->utf8String(), device.registryID(),
        device.hasUnifiedMemory(), device.maxBufferLength(), threads.width,
        threads.height, threads.depth
    );
}

}  // namespace

}  // namespace sl
