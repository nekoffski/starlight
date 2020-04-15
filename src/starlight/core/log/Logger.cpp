#include <starlight/core/log/Logger.h>

INITIALIZE_EASYLOGGINGPP;

namespace starl::core::log {

void initLogger(int argc, char** argv) {
    START_EASYLOGGINGPP(argc, argv);
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "[%datetime] %fbase:%line [%level]: %msg");
}
}