#include "Shader.hh"

#include "starlight/core/Sync.hh"
#include "starlight/core/Time.hh"

namespace sl {

namespace detail {

ShaderImpl::ShaderImpl(
    RendererProxy& proxy, const ShaderDescription& description
)
    : m_proxy(proxy), m_description(description) {}

ShaderImpl::~ShaderImpl() {
    if (m_handle.valid()) {
        m_proxy.destroyShader(m_handle);
    }
}

}  // namespace detail

ShaderRef::ShaderRef(RendererProxy& proxy, const ShaderDescription& description)
    : Ref(proxy, description) {}

}  // namespace sl
