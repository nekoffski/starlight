
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Log.hh"
#include "starlight/platform/Platform.hh"
#include "starlight/renderer/RenderingSystem.hh"

using namespace sl;

int main() {
    Config cfg;

    log::init(log::LoggerOptions{.level = cfg.log.level});
    log::info("Hello world!");

    Platform::logInfo();

    RenderingSystem rs{cfg};
    auto proxy = rs.createRendererProxy();

    return 0;
}
