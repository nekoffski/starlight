#include "MetalRenderPass.hh"

namespace sl {

MetalRenderPassEncoder::MetalRenderPassEncoder(
    MTL::RenderCommandEncoder* encoder
)
    : m_encoder(encoder) {}

MetalRenderPassEncoder::~MetalRenderPassEncoder() { m_encoder->endEncoding(); }

MTL::LoadAction toMetal(LoadOp op) {
    switch (op) {
        case LoadOp::load:
            return MTL::LoadActionLoad;
        case LoadOp::clear:
            return MTL::LoadActionClear;
        default:
            return MTL::LoadActionDontCare;
    }
}

MTL::StoreAction toMetal(StoreOp op) {
    switch (op) {
        case StoreOp::store:
            return MTL::StoreActionStore;
        default:
            return MTL::StoreActionDontCare;
    }
}

}  // namespace sl
