#include "MetalRenderFrameLatch.hh"

namespace sl {

bool MetalRenderFrameLatch::tryAcquire() {
    bool expected = true;
    return m_available.compare_exchange_strong(expected, false);
}

void MetalRenderFrameLatch::release() {
    m_available.store(true);
    m_available.notify_all();
}

void MetalRenderFrameLatch::wait() {
    bool old = false;
    m_available.wait(old);
}

}  // namespace sl
