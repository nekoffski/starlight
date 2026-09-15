#pragma once

#include "Asset.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Error.hh"
#include "starlight/core/Ref.hh"
#include "starlight/renderer/RendererProxy.hh"
#include "starlight/renderer/rhi/Shader.hh"

namespace sl {

namespace detail {

class ShaderImpl : public NonCopyable, public NonMovable {
   public:
    explicit ShaderImpl(
        RendererProxy& proxy, const ShaderDescription& description
    );

    ~ShaderImpl();

    ShaderHandle handle() const { return m_handle; }

    BackendResourceState state() const { return m_state; }
    Error error() const { return m_error; }

   private:
    RendererProxy& m_proxy;
    ShaderDescription m_description;
    ShaderHandle m_handle;
    Error m_error;
    BackendResourceState m_state{BackendResourceState::unknown};
    std::future<Result<ShaderHandle>> m_future;
};

}  // namespace detail

class ShaderRef : public Ref<detail::ShaderImpl> {
   public:
    explicit ShaderRef(
        RendererProxy& proxy, const ShaderDescription& description
    );

    ShaderHandle handle() const { return data().handle(); }
    BackendResourceState state() const { return data().state(); }
    Error error() const { return data().error(); }
};

}  // namespace sl
