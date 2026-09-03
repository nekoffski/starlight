#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "Metal.hh"

#include "starlight/core/Log.hh"

namespace sl {
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

MetalContext::MetalContext() {
    m_device = MTL::CreateSystemDefaultDevice();
    log::expect(m_device != nullptr, "Failed to create Metal device");

    m_queue = m_device->newCommandQueue();
    log::expect(m_queue != nullptr, "Failed to create Metal command queue");

    logDeviceInfo(*m_device);
}

MetalContext::~MetalContext() {
    m_queue->release();
    m_device->release();
}

MTL::Device& MetalContext::device() { return *m_device; }

MTL::CommandQueue& MetalContext::commandQueue() { return *m_queue; }

}  // namespace sl
