#include "Shader.hh"

#include "starlight/core/Sync.hh"
#include "starlight/core/Time.hh"

namespace sl {

namespace detail {

ShaderImpl::ShaderImpl(
    RendererProxy& proxy, const ShaderDescription& description,
    EventLoop& eventLoop
)
    : m_proxy(proxy), m_description(description), m_eventLoop(eventLoop) {
    requestDevice();
}

ShaderImpl::~ShaderImpl() {
    if (m_eventLoopCall) {
        m_eventLoop.remove(*m_eventLoopCall);
    }

    if (m_handle.valid()) {
        m_proxy.destroyShader(m_handle);
    }
}

void ShaderImpl::requestDevice() {
    std::unique_lock lk{m_mutex};

    auto future = m_proxy.createShaderAsync(m_description);

    if (not future) {
        m_error = future.error();
        m_state = BackendResourceState::failed;
        return;
    }

    if (m_eventLoopCall) {
        m_eventLoop.remove(*m_eventLoopCall);
    }

    m_eventLoopCall = m_eventLoop.schedule<PollingEventLoopCall>(
        [&, future = std::move(future.value())]() mutable {
            if (future.valid() && futureReady(future)) {
                m_eventLoopCall.reset();  // is it a race??
                return true;
            }
            return false;
        }
    );
}

}  // namespace detail

ShaderRef::ShaderRef(
    RendererProxy& proxy, const ShaderDescription& description,
    EventLoop& eventLoop
)
    : Ref(proxy, description, eventLoop) {}

}  // namespace sl
