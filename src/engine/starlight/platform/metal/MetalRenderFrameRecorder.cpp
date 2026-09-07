#include "MetalRenderFrameRecorder.hh"

namespace sl {

MetalRenderFrameRecorder::MetalRenderFrameRecorder(
    MetalResourcePool& resourcePool, MTL::CommandBuffer* commandBuffer
)
    : m_resourcePool(resourcePool), m_commandBuffer(commandBuffer) {}

}  // namespace sl
