#include "Application.hh"

#include <starlight/window/Events.hh>
#include <starlight/app/renderPasses/UIRenderPass.hh>
#include <starlight/app/renderPasses/WorldRenderPass.hh>
#include <starlight/app/renderPasses/SkyboxRenderPass.hh>
// #include <starlight/app/renderPasses/LightsDebugRenderPass.hh>
#include <starlight/app/renderPasses/ShadowMapsRenderPass.hh>
#include <starlight/app/renderPasses/GridRenderPass.hh>
#include <starlight/ui/fonts/FontAwesome.hh>
#include <starlight/ui/UI.hh>
#include <starlight/app/scene/Scene.hh>
#include <starlight/window/Input.hh>

#include "Events.hh"

namespace sle {

Application::Application(
  const sl::Config& config, std::optional<std::string> scenePath
) :
    Engine(config), m_eventSentinel(sl::EventProxy::get()),
    m_cameras(sl::Window::get().getFramebufferSize()),
    m_userInterface(
      sl::Window::get().getFramebufferSize(), getScene(), getRenderGraph(),
      getCamera()
    ) {
    if (scenePath) {
        sl::log::info("Loading initial scene: {}", *scenePath);
        setScene(m_sceneParser.deserialize(*scenePath));
        m_userInterface.setScene(getScene());
    }

    initEvents();

    const auto& viewportScale = m_userInterface.getViewportScale();

    getRenderGraph().addPass<sl::SkyboxRenderPass>(viewportScale);
    getRenderGraph().addPass<sl::ShadowMapsRenderPass>();
    getRenderGraph().addPass<sl::WorldRenderPass>(viewportScale);
    getRenderGraph().addPass<sl::GridRenderPass>(viewportScale);
    getRenderGraph().addPass<sl::UIRenderPass>(m_userInterface);
}

void Application::update([[maybe_unused]] float frameTime) {
    auto& io    = ImGui::GetIO();
    auto& input = sl::Input::get();

    input.switchMouseInput(
      io.WantCaptureMouse ? sl::Input::State::off : sl::Input::State::on
    );
    input.switchKeyboardInput(
      io.WantCaptureKeyboard ? sl::Input::State::off : sl::Input::State::on
    );
}

void Application::initEvents() {
    m_eventSentinel
      .add<sl::KeyEvent>([&](const auto& event) {
          if (event.key == SL_KEY_Z) {
              //   m_renderer.setRenderMode(sl::RenderMode::lights);
          } else if (event.key == SL_KEY_X) {
              //   m_renderer.setRenderMode(sl::RenderMode::normals);
          } else if (event.key == SL_KEY_C) {
              //   m_renderer.setRenderMode(sl::RenderMode::standard);
          }
      })
      .add<events::SceneSerialization>([&](const auto& event, auto&& handled) {
          if (event.action == events::SceneSerialization::Action::serialize) {
              editorWriteDebug("Serializing scene: {}", event.path);
              m_sceneParser.serialize(getScene(), event.path);
          } else {
              editorWriteDebug("Deserializing scene: {}", event.path);
              setScene(m_sceneParser.deserialize(event.path));
          }
          handled();
      });
}

}  // namespace sle
