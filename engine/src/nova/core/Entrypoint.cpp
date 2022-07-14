#include "Entrypoint.hpp"

#include <kc/core/Log.h>

#include "Application.h"
#include "Engine.h"
#include "Config.h"

#include "nova/platform/Platform.h"

namespace nova::core::detail {

int mainImpl(int argc, char** argv) {
    kc::core::initLogging("nova-engine");
    LOG_TRACE("Logger intialized, starting engine");

    try {
        LOG_TRACE("Creating Platform instance");
        platform::PlatformProvider platformProvider{defaultConfig};

        auto platform = platformProvider.getPlatform();

        LOG_TRACE("Creating Engine instance");
        Engine engine{platform};

        LOG_TRACE("Creating application instance");
        auto application = createApplication();

        LOG_TRACE("Starting engine");
        engine.run(*application);

    } catch (NovaError& error) {
        LOG_FATAL("Engine exited with error: {}", error.asString());
        return -1;
    }

    LOG_TRACE("Exiting gracefully, see ya");
    return 0;
}

}  // namespace nova::core::detail