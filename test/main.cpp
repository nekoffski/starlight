
#include <starlight/core/log/Logger.h>
#include <starlight/platform/PlatformDetector.h>

int main() {

    starl::core::log::Logger::init();

    STARL_INFO(STARL_GET_PLATFORM_NAME());

    return 0;
}