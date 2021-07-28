#include "sl/app/Engine.h"
#include "sl/app/Entrypoint.h"

#include "StarlightEditor.h"

using namespace sl;

int main(int argc, char** argv) {
    platform::Platform platform {};

    auto engine = app::Engine::Builder {}
                      .setPlatform(std::move(platform))
                      .setConfigFile("./starlight.json")
                      .build();

    auto application = std::make_shared<StarlightEditor>();

    engine->setApplication(std::move(application));
    engine->run();

    return 0;
}
