#include "sl/app/Engine.h"
#include "sl/app/Entrypoint.h"

#include "EditorContext.hpp"

using namespace sl;
using namespace sl::scene;

class StarlightEditor : public sl::app::Application {
public:
    virtual void onStart() override {
        context = createContext<EditorContext>("EditorContext");
        switchContext(context);
    }

private:
    std::shared_ptr<EditorContext> context;
};

int main(int argc, char** argv) {

    platform::Platform platform {};

    auto engine = app::Engine::Builder {}
                      .setPlatform(std::move(platform))
                      .setConfigFile("./starlight.json")
                      .build();

    auto application = std::make_unique<StarlightEditor>();

    engine->setApplication(std::move(application));
    engine->run();

    return 0;
}
