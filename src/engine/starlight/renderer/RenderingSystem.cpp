#include "RenderingSystem.hh"

#include "starlight/platform/Platform.hh"

namespace sl {

namespace {

void validateConfig(const Config& config) {
    log::expect(
        config.renderer.maxFramesInFlight > 0,
        "Invalid configuration: maxFramesInFlight must be positive"
    );
    log::expect(
        config.renderer.maxTextures > 0,
        "Invalid configuration: maxTextures must be positive"
    );
    log::expect(
        config.renderer.maxMeshes > 0,
        "Invalid configuration: maxMeshes must be positive"
    );
    log::expect(
        config.renderer.maxRenderPasses > 0,
        "Invalid configuration: maxRenderPasses must be positive"
    );
    log::expect(
        config.renderer.maxShaders > 0,
        "Invalid configuration: maxShaders must be positive"
    );
}

}  // namespace

RenderingSystem::RenderingSystem(const Config& config)
    : m_config(config),
      m_renderDevice(Platform::createRenderDevice(config)),
      m_renderer(m_config, *m_renderDevice),
      m_renderExecutor(m_renderer) {
    validateConfig(config);
    m_renderExecutor.start();
}

RenderingSystem::~RenderingSystem() { stop(); }

RendererProxy RenderingSystem::createRendererProxy() {
    return RendererProxy{m_renderExecutor.createSubmitter()};
}

void RenderingSystem::stop() {
    m_renderExecutor.stop();
    m_renderDevice->waitIdle();
}

}  // namespace sl
