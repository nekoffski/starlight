#include <kc/core/Log.h>

#include "StarlightEditor.h"
#include "sl/app/Engine.h"
#include "sl/cfg/ConfigLoader.h"
#include "sl/platform/Platform.hpp"
#include "sl/platform/assimp/AssimpModelLoader.h"
#include "sl/platform/gl/OpenGl.h"
#include "sl/platform/glfw/Glfw.h"
#include "sl/platform/stb/StbImage.h"

int main(int argc, char** argv) {
    using namespace sl;
    using namespace sl::platform;
    using namespace sl::app;
    using namespace sl::cfg;

    try {
        kc::core::initLogging("Starlight");

        static const std::string configPath = "./starlight.json";
        auto config                         = ConfigLoader{}.fromFile(configPath).load();

        LOG_DEBUG("Assembling platform module");
        auto platform = Platform::Builder{}
                            .setIO<glfw::Glfw>(config.window)
                            .setGPU<gl::OpenGl>()
                            .setImageFactory<stb::StbImageFactory>()
                            .setModelLoader<assimp::AssimpModelLoader>()
                            .build();

        LOG_DEBUG("Creating engine instance");
        auto engine = Engine::Builder{}.setPlatform(platform.get()).setConfig(&config).build();

        engine->initGlobalState();

        LOG_DEBUG("Creating application instance");
        StarlightEditor starlightEditor;

        engine->setApplication(&starlightEditor);
        engine->run();

    } catch (kc::core::ErrorBase& error) {
        LOG_FATAL("Catched exception in main: {}", error.asString());
        return -1;
    }

    LOG_INFO("Main quits");
    return 0;
}
