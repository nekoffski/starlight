#include "Entrypoint.hpp"

#include <kc/core/Log.h>

#include "Application.h"

namespace nova::detail {

int mainImpl(int argc, char** argv) {
    kc::core::initLogging("nova-engine");
    LOG_TRACE("Logger intialized, starting engine");

    LOG_TRACE("Creating application instance");
    auto application = createApplication();

    LOG_TRACE("Exiting gracefully, see ya");
    return 0;
}

}  // namespace nova::detail