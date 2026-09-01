#include "RenderingSystem.hh"

#include "starlight/platform/Platform.hh"

namespace sl {

RenderingSystem::RenderingSystem(const Config& config)
    : m_config(config), m_renderDevice(Platform::createRenderDevice(config)) {}

RendererProxy RenderingSystem::createRendererProxy() const {
    return RendererProxy{};
}

}  // namespace sl
