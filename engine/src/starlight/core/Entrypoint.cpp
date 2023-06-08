#include "Entrypoint.hpp"

#include <kc/core/Log.h>

#include "Application.h"
#include "Engine.h"
#include "Config.h"

#include "starlight/platform/Platform.h"

#include "starlight/res/Resource.h"

namespace sl::core::detail {

int mainImpl(int argc, char** argv) {
    kc::core::initLogging("starlight-engine");
    LOG_DEBUG("Logger intialized, starting engine");

    try {
        LOG_DEBUG("Creating Platform instance");
        platform::PlatformProvider platformProvider{defaultConfig};

        auto platform = platformProvider.getPlatform();

        LOG_DEBUG("Creating Engine instance");
        Engine engine{platform};

        LOG_DEBUG("Creating application instance");
        auto application = createApplication();

        LOG_DEBUG("Starting engine");
        engine.run(*application);

    } catch (NovaError& error) {
        LOG_FATAL("Engine exited with error: {}", error.asString());
        return -1;
    }

    LOG_DEBUG("Exiting gracefully, see ya");
    return 0;
}

}  // namespace sl::core::detail