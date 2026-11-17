

#include <algorithm>
#include <kcore/io/FileSystem.hh>

#include "starlight/asset/AssetSystem.hh"
#include "starlight/asset/Texture.hh"
#include "starlight/camera/OrbitCameraController.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Time.hh"
#include "starlight/event/EventSystem.hh"
#include "starlight/event/Events.hh"
#include "starlight/geometry/ProceduralGeometry.hh"
#include "starlight/math/Math.hh"
#include "starlight/platform/Platform.hh"
#include "starlight/platform/sdl/SDLWindow.hh"
#include "starlight/renderer/RenderingSystem.hh"
#include "starlight/renderer/frontend/RenderRequestBuilder.hh"
#include "starlight/renderer/shading/MaterialBuilder.hh"
#include "starlight/window/WindowEvents.hh"

using namespace sl;

namespace {

Result<Material> makeCheckerboardMaterial(
    const ShaderInterface& shader, TextureHandle texture, SamplerHandle sampler
) {
    // Shader table indices are logical IDs, not native Metal bindings.
    const std::array<u32, 4> drawParameters{texture.id, sampler.id, 0, 0};
    return MaterialBuilder{shader}
        .addParameter("draw", drawParameters)
        .addTextureDependency(texture)
        .addSamplerDependency(sampler)
        .build();
}

}  // namespace

int main() {
    Config cfg;

    log::init({
        .level = cfg.log.level,
        .formatPattern = "%^[%d-%m-%Y %T] [Th: %t] %-7l [starlight]: %v %$",
    });
    log::info("Hello world! {}", kc::Path::cwd().str());

    Platform::logInfo();
    EventSystem es;

    auto window = std::make_shared<SDLWindow>(cfg, es.createBus());
    RenderingSystem rs{cfg};
    AssetSystem as{cfg, rs.createRendererProxy()};

    auto proxy = rs.createRendererProxy();
    auto mainBus = es.createBus();
    auto target = proxy.createRenderTarget(window);
    log::expect(target);

    std::atomic_bool running{true};

    mainBus.on<QuitRequestedEvent>([&](const auto& event) {
        log::info("Quit requested: {}", event.reason);
        running = false;
    });

    mainBus.on<WindowResizedEvent>([&](const auto& event) {
        log::info("Window resized: {}x{}", event.width, event.height);
    });

    auto shader = as.loadShader("shaders/simple.metallib");
    log::expect(shader);

    const auto cubeData = ProceduralGeometry::cube();
    auto geometry = as.createGeometry(cubeData);
    log::expect(geometry);

    auto checkerboard = makeCheckerboard(64, 8);
    log::expect(checkerboard);
    auto texture = as.createTexture(*checkerboard);
    log::expect(texture);
    auto sampler = as.createSampler({});
    log::expect(sampler);
    auto smoothSampler = as.createSampler({.filter = TextureFilter::linear});
    log::expect(smoothSampler);

    log::expect(as.waitForPending());
    const auto textureHandle = texture->handle();
    const auto samplerHandle = sampler->handle();
    const auto smoothSamplerHandle = smoothSampler->handle();
    const auto primitiveHandle = geometry->handle();
    log::expect(textureHandle);
    log::expect(samplerHandle);
    log::expect(smoothSamplerHandle);
    log::expect(primitiveHandle);

    const auto shaderInterface = shader->interface();
    log::expect(shaderInterface);
    const auto material = makeCheckerboardMaterial(
        *shaderInterface, *textureHandle, *samplerHandle
    );
    const auto smoothMaterial = makeCheckerboardMaterial(
        *shaderInterface, *textureHandle, *smoothSamplerHandle
    );
    log::expect(material);
    log::expect(smoothMaterial);
    const std::array sectionMaterials{
        std::cref(*material), std::cref(*smoothMaterial)
    };
    constexpr u32 indicesPerFace = 6;

    constexpr f32 wheelScaleStep = 0.05f;
    f32 geometryScale = 1.0f;
    f32 angle = 0.0f;
    bool rotating = true;
    EulerCamera camera;
    OrbitCameraController cameraController;
    log::expect(camera.setPose({.position = Vec3f{0.0f, 0.0f, 3.0f}}));

    mainBus.on<KeyPressedEvent>([&](const auto& event) {
        if (event.window == window->id() && event.key == Key::escape &&
            not event.repeat) {
            running = false;
        }
    });
    mainBus.on<MouseButtonPressedEvent>([&](const auto& event) {
        if (event.window == window->id() && event.button == MouseButton::left) {
            rotating = not rotating;
        }
    });
    mainBus.on<MouseWheelEvent>([&](const auto& event) {
        if (event.window == window->id()) {
            geometryScale = std::clamp(
                geometryScale + event.delta.y * wheelScaleStep, 0.1f, 1.5f
            );
        }
    });

    auto previousUpdate = Clock::now();
    while (running) {
        log::expect(as.update());
        window->pollEvents();
        es.dispatch();

        if (not running) {
            break;
        }
        const auto now = Clock::now();
        const auto deltaTime =
            std::chrono::duration<f32>(now - previousUpdate).count();
        previousUpdate = now;
        log::expect(cameraController.update(camera, *window, deltaTime));
        if (rotating) {
            angle += deltaTime;
        }
        const auto size = window->drawableSize();
        if (size.x == 0 || size.y == 0) {
            std::this_thread::sleep_for(50ms);
            continue;
        }
        const auto matrices = camera.matrices(f32(size.x) / f32(size.y));
        log::expect(matrices);
        RenderRequestBuilder requestBuilder;
        requestBuilder
            .addView({
                .target = *target,
                .parameters = {ShaderParameterBlock::fromValue(
                    "view", *matrices
                )},
            })
            .setClearColor({0.2f, 0.2f, 0.6f, 1.0f});
        auto modelToWorld = glm::rotate(
            Mat4f{1.0f}, angle, glm::normalize(Vec3f{0.5f, 1.0f, 0.25f})
        );
        modelToWorld = glm::scale(modelToWorld, Vec3f{geometryScale});
        for (u32 firstIndex = 0; firstIndex < cubeData.indexCount;
             firstIndex += indicesPerFace) {
            const auto materialIndex =
                (firstIndex / indicesPerFace) % sectionMaterials.size();
            requestBuilder.addDraw({
                .material = sectionMaterials[materialIndex].get(),
                .primitives =
                    {{.primitive = *primitiveHandle,
                      .indexRange = IndexRange{firstIndex, indicesPerFace}}},
                .parameters = {
                    .blocks = {ShaderParameterBlock::fromValue(
                        "transform", modelToWorld
                    )}
                },
            });
        }

        auto request = requestBuilder.build();
        log::expect(request);
        const auto r = proxy.submit(*request);
        if (not r) {
            if (r.error().code() == ErrorCode::tooManyFrameRequests) {
                log::debug(
                    "Too many frame requests, waiting for renderer to catch "
                    "up..."
                );
                std::this_thread::sleep_for(50ms);
            } else {
                log::error("Failed to submit render request: {}", r.error());
                break;
            }
        }
    }

    return 0;
}
