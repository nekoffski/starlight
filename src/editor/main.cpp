#include "sl/app/ConfigLoader.h"
#include "sl/app/Engine.h"
#include <kc/core/Log.h>

#include "sl/platform/Platform.hpp"
#include "sl/platform/assimp/AssimpModelLoader.h"
#include "sl/platform/gl/OpenGl.h"
#include "sl/platform/glfw/Glfw.h"
#include "sl/platform/stb/StbImage.h"

#include "StarlightEditor.h"

int main(int argc, char** argv) {
    using namespace sl;
    using namespace sl::platform;
    using namespace sl::app;

    kc::core::initLogging("Starlight");

    static const std::string configPath = "./starlight.json";
    auto config = ConfigLoader {}.loadFromFile(configPath);

    auto platform = Platform::Builder {}
                        .setIO<glfw::Glfw>(config.window)
                        .setGPU<gl::OpenGl>()
                        .setImageFactory<stb::StbImageFactory>()
                        .setModelLoader<assimp::AssimpModelLoader>()
                        .build();

    auto engine = Engine::Builder {}
                      .setPlatform(platform.get())
                      .setConfig(&config)
                      .build();

    engine->initGlobalState();

    auto application = std::make_unique<StarlightEditor>();

    engine->setApplication(std::move(application));
    engine->run();

    return 0;
}
