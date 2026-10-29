
#include "starlight/asset/AssetSystem.hh"
#include "starlight/asset/Texture.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/FileSystem.hh"
#include "starlight/core/Log.hh"
#include "starlight/core/Time.hh"
#include "starlight/event/EventSystem.hh"
#include "starlight/event/Events.hh"
#include "starlight/math/Math.hh"
#include "starlight/platform/Platform.hh"
#include "starlight/platform/sdl/SDLWindow.hh"
#include "starlight/renderer/RenderingSystem.hh"

using namespace sl;

int main() {
    Config cfg;

    log::init(log::LoggerOptions{.level = cfg.log.level});
    log::info("Hello world! {}", Path::cwd().str());

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

    shader->waitForDevice();

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
    auto texture = proxy.createTexture(*checkerboard);
    log::expect(texture);
    auto sampler = proxy.createSampler({});
    log::expect(sampler);
    DEFER {
        proxy.destroyTexture(*texture);
        proxy.destroySampler(*sampler);
    };

    // Shader table indices are logical resource IDs, not native Metal bindings.
    const std::array<u32, 4> drawParameters{texture->id, sampler->id, 0, 0};

    RenderRequest req{
        .scene =
            RenderScene{
                .items = {RenderItem{
                    .primitives = {*primitive},
                    .shader = shader->handle(),
                    .parameters =
                        {
                            ShaderParameterBlock::fromValue(
                                "draw", drawParameters
                            ),
                            ShaderParameterBlock::fromValue(
                                "transform", Mat4f{1.0f}
                            ),
                        }
                }}
            },
        .views = {RenderView{.target = *target}},
        .clearColor = {0.2f, 0.2f, 0.6f, 1.0f},
        .resources = {.textures = {*texture}, .samplers = {*sampler}}
    };

    const auto animationStart = Clock::now();
    while (running) {
        window->pollEvents();
        es.dispatch();

        const auto elapsed =
            std::chrono::duration<f32>(Clock::now() - animationStart).count();
        const auto modelToClip =
            glm::rotate(Mat4f{1.0f}, elapsed, Vec3f{0.0f, 0.0f, 1.0f});
        req.scene.items.front().parameters.back() =
            ShaderParameterBlock::fromValue("transform", modelToClip);

        if (auto res = proxy.submit(req); not res) {
            if (res.error().code() == ErrorCode::tooManyFrameRequests) {
                log::debug(
                    "Too many frame requests, waiting for renderer to catch "
                    "up..."
                );
                std::this_thread::sleep_for(50ms);
            } else {
                log::error("Failed to submit render request: {}", res.error());
                break;
            }
        }
    }

    return 0;
}
