#include "MetalRenderFrameLatch.hh"

namespace sl {

bool MetalRenderFrameLatch::tryAcquire() {
    bool expected{true};
    return m_available.compare_exchange_strong(expected, false);
}

void MetalRenderFrameLatch::release() { m_available.store(true); }

}  // namespace sl
