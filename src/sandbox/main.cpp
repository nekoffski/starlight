
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Log.hh"
#include "starlight/core/Time.hh"
#include "starlight/event/EventSystem.hh"
#include "starlight/event/Events.hh"
#include "starlight/platform/Platform.hh"
#include "starlight/platform/sdl/SDLWindow.hh"
#include "starlight/renderer/RenderingSystem.hh"

using namespace sl;

int main() {
    Config cfg;

    log::init(log::LoggerOptions{.level = cfg.log.level});
    log::info("Hello world!");

    Platform::logInfo();
    EventSystem es;

    auto window = std::make_shared<SDLWindow>(cfg, es.createBus());
    RenderingSystem rs{cfg};

    auto proxy = rs.createRendererProxy();
    auto mainBus = es.createBus();

    std::atomic_bool running{true};

    mainBus.on<QuitRequestedEvent>([&](const auto& event) {
        log::info("Quit requested: {}", event.reason);
        running = false;
    });

    while (running) {
        window->pollEvents();
        es.dispatch();

        RenderRequest req;

        if (auto res = proxy.submit(req); not res) {
            if (res.error().code() == ErrorCode::tooManyFramesInFlight) {
                log::debug(
                    "Too many frames in flight, waiting for renderer to catch "
                    "up..."
                );
                std::this_thread::sleep_for(50ms);
            } else {
                log::error(
                    "Failed to submit render request: {}", res.error().message()
                );
                break;
            }
        }
    }

    return 0;
}
