#pragma once

#include <shared_mutex>

#include "Asset.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Error.hh"
#include "starlight/core/Ref.hh"
#include "starlight/renderer/RendererProxy.hh"
#include "starlight/renderer/rhi/Shader.hh"
#include "starlight/runtime/EventLoop.hh"

namespace sl {

namespace detail {

class ShaderImpl : public NonCopyable, public NonMovable {
   public:
    explicit ShaderImpl(
        RendererProxy& proxy, const ShaderDescription& description,
        EventLoop& eventLoop
    );

    ~ShaderImpl();

    ShaderHandle handle() const {
        std::shared_lock lk{m_mutex};
        return m_handle;
    }

    BackendResourceState state() const {
        std::shared_lock lk{m_mutex};
        return m_state;
    }

    Error error() const {
        std::shared_lock lk{m_mutex};
        return m_error;
    }

    void waitForDevice();
    void requestUpdate();

   private:
    void requestDevice();

    RendererProxy& m_proxy;
    ShaderDescription m_description;
    EventLoop& m_eventLoop;
    ShaderHandle m_handle;
    Error m_error;
    Opt<Tag<Str>> m_eventLoopCall;
    BackendResourceState m_state{BackendResourceState::unknown};

    mutable std::shared_mutex m_mutex;
};

}  // namespace detail

class ShaderRef : public Ref<detail::ShaderImpl> {
   public:
    explicit ShaderRef(
        RendererProxy& proxy, const ShaderDescription& description,
        EventLoop& eventLoop
    );

    ShaderHandle handle() const { return data().handle(); }
    BackendResourceState state() const { return data().state(); }

    void requestUpdate() { data().requestUpdate(); }

    Error error() const { return data().error(); }
    void waitForDevice() { data().waitForDevice(); }
};

}  // namespace sl
