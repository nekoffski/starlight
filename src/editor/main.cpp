#include "sl/application/Entrypoint.hpp"


#include "EditorContext.hpp"


using namespace sl;
using namespace sl::scene;

class StarlightEditor : public sl::application::Application {
    using sl::application::Application::Application;

public:
    void preInit() override {
        initPaths();
    }

    virtual void onStart() override {
		context = createContext<EditorContext>(sl::application::context::CONTEXT_3D);
        switchContext(context);
    }

    void initPaths() {
        sl::core::path::PathManager::registerResourcePath<sl::platform::shader::Shader>(SHADERS_DIR);
        sl::core::path::PathManager::registerResourcePath<sl::platform::texture::Texture>(TEXTURES_DIR);
        sl::core::path::PathManager::registerResourcePath<sl::platform::texture::Cubemap>(CUBEMAPS_DIR);
        sl::core::path::PathManager::registerResourcePath<sl::geometry::Model>(MODELS_DIR);
    }

private:
    std::shared_ptr<EditorContext> context;
};

STARLIGHT_ENTRYPOINT(StarlightEditor);



