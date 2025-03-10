#include "Engine.hh"

namespace sl {

Engine::Engine(const Config& config) :
    m_globals(config), m_isRunning(true), m_eventProxy(m_eventBroker.getProxy()),
    m_eventSentinel(m_eventProxy), m_input(m_window.getImpl()),
    m_camera(&m_defaultCamera), m_scene(SharedPtr<Scene>::create()),
    m_renderGraph(SharedPtr<RenderGraph>::create(m_renderer)),
    m_meshFactory(m_renderer.getVertexBuffer(), m_renderer.getIndexBuffer()) {
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
    renderPacket.camera = getCamera();
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

Scene* Engine::getScene() { return m_scene.get(); }

void Engine::setScene(SharedPtr<Scene> scene) { m_scene = std::move(scene); }

RenderGraph* Engine::getRenderGraph() { return m_renderGraph.get(); }

void Engine::setRenderGraph(SharedPtr<RenderGraph> renderGraph) {
    m_renderGraph = renderGraph;
}

Camera* Engine::getCamera() { return m_camera; }

void Engine::setCamera(Camera* camera) { m_camera = camera; }

void Engine::useDefaultCamera() { m_camera = &m_defaultCamera; }

void Engine::updateFrame(float frameTime) {
    m_camera->update(frameTime);
    update(frameTime);
}

void Engine::initEvents() {
    m_eventSentinel
      .add<sl::KeyEvent>([&](auto& event) {
          if (event.action == sl::KeyAction::press && event.key == SL_KEY_ESCAPE) {
              log::debug("ESC pressed, quit requested");
              m_isRunning = false;
          }
      })
      .add<QuitEvent>([&](auto& event) {
          log::debug("Got QuitEvent reason: {}, quit requested", event.reason);
          m_isRunning = false;
      });
}

}  // namespace sl
