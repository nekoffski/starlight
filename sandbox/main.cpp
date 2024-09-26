

#include <csignal>

#include "starlight/core/Core.hh"
#include "starlight/core/window/Window.hh"
#include "starlight/core/Context.hh"
#include "starlight/core/event/Quit.hh"

#include "starlight/renderer/views/WorldRenderView.hh"
#include "starlight/renderer/camera/EulerCamera.hh"
#include "starlight/renderer/views/SkyboxRenderView.hh"
#include "starlight/renderer/RendererFrontend.hh"
#include "starlight/renderer/RenderGraph.hh"
#include "starlight/renderer/gpu/Shader.hh"

#include "starlight/renderer/PointLight.hh"

static std::atomic_bool isRunning = true;

/*

TODO:
    - [18-09-2024 23:30:36] [Th: 665267] warning [starlight]: Could not find record
to release with name:  m;�lUp�O�lUp�O�lU - [Resource.hh:103]
    - Refactor Renderer
        - getFramebuffer returns texture - this is invalid!
    - Don't use ResourceManagers for storing default stuff, just create as objects
    - Shader as a part of render packet
    - Simplify components
    - Refactor Scene

    - Builders for XYZ::Properties structures
    - LoadObj should return a tree of Entities
        - renderer needs a composite of Mesh, Material, ModelMatrix, Shader
        - RenderTree?
    - wrap up ResourceRef
*/

int main() {
    std::signal(SIGINT, []([[maybe_unused]] int) { isRunning = false; });

    sl::Context context;
    sl::RendererFrontend renderer{ context };

    auto& window            = context.getWindow();
    const auto viewportSize = window.getFramebufferSize();

    auto& eventProxy = sl::EventProxy::get();
    sl::EventHandlerSentinel sentinel{ eventProxy };

    sentinel
      .pushHandler<sl::QuitEvent>([&](const auto& ev) {
          isRunning = false;
          return sl::EventChainBehaviour::propagate;
      })
      .pushHandler<sl::KeyEvent>([&](const auto& ev) {
          if (ev.key == SL_KEY_ESCAPE) isRunning = false;
          return sl::EventChainBehaviour::propagate;
      });

    sl::EulerCamera camera(sl::EulerCamera::Properties{
      .target       = sl::Vec3<sl::f32>{ 0.0f },
      .radius       = 5.0f,
      .viewportSize = viewportSize,
    });

    auto& rendererBackend = renderer.getRendererBackend();
    auto skybox           = sl::Skybox::load("skybox2/skybox");
    auto worldShader      = sl::Shader::load("Builtin.Shader.Material");

    auto renderGraph =
      sl::RenderGraph::Builder{ rendererBackend, viewportSize }
        .addView<sl::SkyboxRenderView>(skybox.get())
        .addView<sl::WorldRenderView>(worldShader.get())
        .build();

    sl::RenderPacket renderPacket;

    renderPacket.viewport = sl::Rect2<sl::u32>{
        sl::Vec2<sl::u32>{0u, 0u},
        viewportSize
    };
    renderPacket.camera = &camera;

    auto material = sl::Material::load("Builtin.Material.Test");

    sl::RenderEntity entity = {
        .worldTransform = sl::math::scale(
          sl::identityMatrix, sl::Vec3<sl::f32>{ 0.25f, 0.25f, 0.25f }
        ),
        .mesh     = sl::Mesh::getCube().get(),
        .material = material.get(),
    };

    sl::PointLight light;
    light.position             = sl::Vec4<sl::f32>{ 0.0f, 3.75f, 0.0f, 1.0f };
    light.attenuationFactors.x = 0.0f;

    renderPacket.entities.push_back(entity);
    renderPacket.pointLights.push_back(light);

    while (isRunning) {
        context.beginFrame([&](float deltaTime) {
            renderer.renderFrame(deltaTime, renderPacket, *renderGraph);
            camera.update(deltaTime);
        });
    }

    return 0;
}
