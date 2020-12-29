#include "Signal.h"

#include "sl/core/Logger.h"

static sl::core::sig::SignalHandler* signalHandler = nullptr;

static inline void signalHandlerWrapper(int signal) {
    SL_ASSERT(signalHandler != nullptr, "Signal handler is not set up");
    SL_INFO("received signal: {}", signal);
    signalHandler->onSignal(signal);
}

namespace sl::core::sig {

void setupSignalHandler(core::types::NotNullPtr<SignalHandler> sigHandler) {
    SL_INFO("setting up signal handler");
    SL_ASSERT(signalHandler == nullptr, "Could not set signal handler twice");

    signalHandler = sigHandler;

    std::signal(SIGINT, signalHandlerWrapper);
    std::signal(SIGTERM, signalHandlerWrapper);
}
}
