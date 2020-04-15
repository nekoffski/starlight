#include <starlight/core/log/Logger.h>
#include <starlight/core/sig/Signal.h>

#include <iostream>

static starl::core::sig::SignalHandler* signalHandler{ nullptr };

static inline void signalHandlerWrapper(int signal) {
    LOG(INFO) << "received signal: " << signal;
    signalHandler->onSignal(signal);
}

namespace starl::core::sig {

void setupSignalHandler(misc::types::NotNullPtr<SignalHandler> sigHandler) {
    LOG(INFO) << "setting up signal handler";
    if (signalHandler != nullptr) {
        LOG(ERROR) << "could not set signal handle twice";
        throw std::logic_error("signal handler instance already exists");
    }
    signalHandler = sigHandler;

    std::signal(SIGINT, signalHandlerWrapper);
    std::signal(SIGTERM, signalHandlerWrapper);
}
}