
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Log.hh"
#include "starlight/platform/Platform.hh"
#include "starlight/platform/sdl/SDLWindow.hh"
#include "starlight/renderer/RenderingSystem.hh"

using namespace sl;

int main() {
    Config cfg;

    log::init(log::LoggerOptions{.level = cfg.log.level});
    log::info("Hello world!");

    auto window = std::make_shared<SDLWindow>(cfg);

    Platform::logInfo();
    RenderingSystem rs{cfg};

    auto proxy = rs.createRendererProxy();
    auto output = proxy.createRenderOutput(window);

    log::expect(output);

    return 0;
}
