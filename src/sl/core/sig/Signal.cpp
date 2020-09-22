#include "Signal.h"

#include "sl/core/log/Logger.h"

static sl::core::sig::SignalHandler* signalHandler{ nullptr };

static inline void signalHandlerWrapper(int signal) {
    SL_INFO("received signal: {}", signal);
    signalHandler->onSignal(signal);
}

namespace sl::core::sig {

void setupSignalHandler(misc::types::NotNullPtr<SignalHandler> sigHandler) {
    SL_INFO("setting up signal handler");
    if (signalHandler != nullptr) {
        SL_ERROR("could not set signal handle twice");
        throw std::logic_error("signal handler instance already exists");
    }
    signalHandler = sigHandler;

    std::signal(SIGINT, signalHandlerWrapper);
    std::signal(SIGTERM, signalHandlerWrapper);
}
}