
#include <algorithm>
#include <kcore/io/FileSystem.hh>

#include "starlight/asset/AssetSystem.hh"
#include "starlight/asset/Texture.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Time.hh"
#include "starlight/event/EventSystem.hh"
#include "starlight/event/Events.hh"
#include "starlight/math/Math.hh"
#include "starlight/platform/Platform.hh"
#include "starlight/platform/sdl/SDLWindow.hh"
#include "starlight/renderer/RenderingSystem.hh"
#include "starlight/renderer/frontend/RenderRequestBuilder.hh"
#include "starlight/renderer/shading/MaterialBuilder.hh"
#include "starlight/window/WindowEvents.hh"

using namespace sl;

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

    auto primitiveData =
        PrimitiveUploadDataBuilder<Vertex3>{}
            .addVertex(Vec3f{0.0f, 0.5f, 0.0f}, Vec3f{1.0f, 0.0f, 0.0f})
            .addVertex(Vec3f{-0.5f, -0.5f, 0.0f}, Vec3f{0.0f, 0.0f, 1.0f})
            .addVertex(Vec3f{0.5f, -0.5f, 0.0f}, Vec3f{0.0f, 1.0f, 0.0f})
            .addIndices({0, 1, 2})
            .build();

    auto primitive = proxy.createPrimitive(primitiveData);
    log::expect(primitive);

    auto checkerboard = makeCheckerboard(64, 8);
    log::expect(checkerboard);
    auto texture = as.createTexture(*checkerboard);
    log::expect(texture);
    auto sampler = as.createSampler({});
    log::expect(sampler);

    log::expect(as.waitForPending());
    const auto textureHandle = texture->handle();
    const auto samplerHandle = sampler->handle();
    log::expect(textureHandle);
    log::expect(samplerHandle);

    // Shader table indices are logical resource IDs, not native Metal bindings.
    const std::array<u32, 4> drawParameters{
        textureHandle->id, samplerHandle->id, 0, 0
    };

    auto shaderInterface = shader->interface();
    log::expect(shaderInterface);
    auto material = MaterialBuilder{std::move(*shaderInterface)}
                        .addParameter("draw", drawParameters)
                        .addTextureDependency(*textureHandle)
                        .addSamplerDependency(*samplerHandle)
                        .build();
    log::expect(material);

    constexpr std::array horizontalOffsets{-0.5f, 0.5f};
    constexpr f32 movementSpeed = 0.8f;
    constexpr f32 wheelScaleStep = 0.05f;
    f32 triangleScale = 0.6f;
    f32 angle = 0.0f;
    bool rotating = true;
    Vec2f sceneOffset{0.0f};

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
            triangleScale = std::clamp(
                triangleScale + event.delta.y * wheelScaleStep, 0.1f, 1.5f
            );
        }
    });

    auto previousUpdate = Clock::now();
    while (running) {
        log::expect(as.update());
        window->pollEvents();
        es.dispatch();

        const Window& input = *window;
        if (not running) {
            break;
        }
        const auto now = Clock::now();
        const auto deltaTime =
            std::chrono::duration<f32>(now - previousUpdate).count();
        previousUpdate = now;
        const Vec2f movement{
            f32(input.isPressed(Key::d)) - f32(input.isPressed(Key::a)),
            f32(input.isPressed(Key::w)) - f32(input.isPressed(Key::s))
        };
        sceneOffset += movement * movementSpeed * deltaTime;
        if (rotating) {
            angle += deltaTime;
        }
        RenderRequestBuilder requestBuilder;
        requestBuilder.addView({.target = *target})
            .setClearColor({0.2f, 0.2f, 0.6f, 1.0f});
        for (const auto offset : horizontalOffsets) {
            auto modelToClip = glm::translate(
                Mat4f{1.0f}, Vec3f{offset + sceneOffset.x, sceneOffset.y, 0.0f}
            );
            modelToClip =
                glm::rotate(modelToClip, angle, Vec3f{0.0f, 0.0f, 1.0f});
            modelToClip = glm::scale(modelToClip, Vec3f{triangleScale});
            requestBuilder.addDraw({
                .material = *material,
                .primitives = {*primitive},
                .parameters = {
                    .blocks = {ShaderParameterBlock::fromValue(
                        "transform", modelToClip
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
