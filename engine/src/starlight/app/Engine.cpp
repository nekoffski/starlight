#include "Engine.hh"

namespace sl {

Engine::Engine(const Config& config)
    : m_globals(config)
    , m_isRunning(true)
    , m_eventProxy(m_eventBroker.getProxy())
    , m_eventSentinel(m_eventProxy)
    , m_input(m_window.getImpl())
    , m_camera(&m_defaultCamera)
    , m_scene(kstd::makeShared<Scene>())
    , m_renderGraph(kstd::makeShared<RenderGraph>(m_renderer))
    , m_meshFactory(m_renderer.getVertexBuffer(), m_renderer.getIndexBuffer()) {
    initEvents();
}

int Engine::run() {
    while (m_isRunning) {
        const auto frameTime = beginFrame();
        updateFrame(frameTime);
        render();
        endFrame();
    }

    return 0;
}

void Engine::render() {
    auto renderPacket   = m_scene->getRenderPacket();
    renderPacket.camera = &getCamera();
    m_renderGraph->render(renderPacket);
}

float Engine::beginFrame() {
    m_taskQueue.dispatchQueue(TaskQueue::Type::preFrame);

    m_window.getImpl().update();
    m_eventBroker.dispatch();

    return m_clock.getDeltaTime();
}

void Engine::endFrame() {
    m_window.getImpl().swapBuffers();

    m_clock.update();
    m_input.update();

    m_taskQueue.dispatchQueue(TaskQueue::Type::postFrame);
}

void Engine::stop() { m_isRunning = false; }

Scene& Engine::getScene() { return *m_scene; }

void Engine::setScene(kstd::SharedPtr<Scene> scene) { m_scene = std::move(scene); }

RenderGraph& Engine::getRenderGraph() { return *m_renderGraph; }

void Engine::setRenderGraph(kstd::SharedPtr<RenderGraph> renderGraph) {
    m_renderGraph = renderGraph;
}

Camera& Engine::getCamera() { return *m_camera; }

void Engine::setCamera(Camera& camera) { m_camera = &camera; }

void Engine::useDefaultCamera() { m_camera = &m_defaultCamera; }

void Engine::updateFrame(float frameTime) {
    m_camera->update(frameTime);
    update(frameTime);
}

void Engine::initEvents() {
    m_eventSentinel.add<QuitEvent>([&](auto& event) {
        log::debug("Got QuitEvent reason: {}, quit requested", event.reason);
        m_isRunning = false;
    });
}

}  // namespace sl
