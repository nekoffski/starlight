#include "RenderingSystem.hh"

#include "starlight/platform/Platform.hh"

namespace sl {

RenderingSystem::RenderingSystem(const Config& config)
    : m_config(config),
      m_renderDevice(Platform::createRenderDevice(config)),
      m_renderer(m_config, *m_renderDevice),
      m_renderExecutor(m_renderer) {
    m_renderExecutor.start();
}

RenderingSystem::~RenderingSystem() { stop(); }

RendererProxy RenderingSystem::createRendererProxy() {
    return RendererProxy{m_renderExecutor.createSubmitter()};
}

void RenderingSystem::stop() { m_renderExecutor.stop(); }

}  // namespace sl
