#include "MetalDevice.hh"

namespace sl {

MetalDevice::MetalDevice(const Config& config, Guard guard)
    : m_config(config), m_resourcePool(config, m_ctx) {
    log::trace("MetalDevice created");

    for (u8 i = 0; i < m_config.renderer.maxFramesInFlight; ++i) {
        m_frames.push_back(std::make_unique<MetalRenderFrameFence>());
    }
}

MetalDevice::~MetalDevice() { log::trace("MetalDevice destroyed"); }

std::unique_ptr<MetalDevice> MetalDevice::create(const Config& config) {
    return std::make_unique<MetalDevice>(config, Guard{});
}

Result<void> MetalDevice::trySubmitFrame(RecordFrame record) {
    auto& fence = m_frames[m_nextFrameSlot];

    if (not fence->tryAcquire()) {
        return Error::unexpected(
            ErrorCode::tooManyFramesInFlight, "Frame context is busy"
        );
    }

    GuardCall fenceReleaser{[&]() { fence->release(); }};

    auto* commandBuffer = m_ctx.commandQueue().commandBuffer();

    if (not commandBuffer) {
        return Error::unexpected(
            ErrorCode::renderEncodingFailed, "Failed to create command buffer"
        );
    }

    MetalRenderFrameRecorder recorder{
        m_resourcePool, commandBuffer, m_nextFrameSlot
    };

    if (auto res = record(recorder); not res) {
        return Error::unexpected(res.error());
    }

    commandBuffer->addCompletedHandler(
        [fence = fence.get()](MTL::CommandBuffer*) { fence->release(); }
    );
    commandBuffer->commit();

    fenceReleaser.dismiss();
    m_nextFrameSlot =
        (m_nextFrameSlot + 1) % m_config.renderer.maxFramesInFlight;

    return {};
}

}  // namespace sl