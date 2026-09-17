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

void ShaderImpl::waitForDevice() {
    for (;;) {
        {
            // TODO: add CV + mutex instead of busy waiting here
            std::shared_lock lk{m_mutex};
            if (m_state != BackendResourceState::queued) {
                break;
            }
        }
        std::this_thread::sleep_for(150ms);
    }
}

void ShaderImpl::requestUpdate() { requestDevice(); }

void ShaderImpl::requestDevice() {
    std::unique_lock lk{m_mutex};

    auto future = m_proxy.createShaderAsync(m_description);

    if (not future) {
        m_error = future.error();
        m_state = BackendResourceState::failed;
        return;
    }

    m_error.reset();
    m_state = BackendResourceState::queued;

    if (m_eventLoopCall) {
        m_eventLoop.remove(*m_eventLoopCall);
    }

    m_eventLoopCall = m_eventLoop.schedule<PollingEventLoopCall>(
        [&, future = std::move(future.value())]() mutable {
            if (not futureReady(future)) {
                return false;
            }

            std::unique_lock lk{m_mutex};

            if (auto maybeShaderHandle = future.get(); maybeShaderHandle) {
                m_handle = maybeShaderHandle.value();
                m_state = BackendResourceState::created;
            } else {
                log::warn(
                    "Backend request to create shader failed: {}",
                    maybeShaderHandle.error()
                );
                m_error = maybeShaderHandle.error();
                m_state = BackendResourceState::failed;
            }

            m_eventLoopCall.reset();
            return true;
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
