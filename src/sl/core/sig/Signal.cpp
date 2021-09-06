#include "Signal.h"

#include <kc/core/Log.h>

static sl::core::sig::SignalHandler* signalHandler = nullptr;

static inline void signalHandlerWrapper(int signal) {
    ASSERT(signalHandler != nullptr, "Signal handler is not set up");
    LOG_INFO("received signal: {}", signal);
    signalHandler->onSignal(signal);
}

namespace sl::core::sig {

void setupSignalHandler(core::NotNullPtr<SignalHandler> sigHandler) {
    LOG_INFO("setting up signal handler");
    ASSERT(signalHandler == nullptr, "Could not set signal handler twice");

    signalHandler = sigHandler;

    std::signal(SIGINT, signalHandlerWrapper);
    std::signal(SIGTERM, signalHandlerWrapper);
}
}
