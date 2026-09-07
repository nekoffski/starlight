#include "MetalRenderFrameFence.hh"

namespace sl {

bool MetalRenderFrameFence::tryAcquire() {
    bool expected{true};
    return m_available.compare_exchange_strong(expected, false);
}

void MetalRenderFrameFence::release() { m_available.store(true); }

}  // namespace sl
