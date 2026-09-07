#include "MetalRenderFrameRecorder.hh"

namespace sl {

MetalRenderFrameRecorder::MetalRenderFrameRecorder(
    MetalResourcePool& resourcePool, MTL::CommandBuffer* commandBuffer,
    u8 frameSlot
)
    : m_resourcePool(resourcePool),
      m_commandBuffer(commandBuffer),
      m_frameSlot(frameSlot) {}

}  // namespace sl
