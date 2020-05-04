#pragma once

#include <starlight/asset/AssetManager.hpp>
#include <starlight/platform/window/Viewport.h>
#include <starlight/rendering/renderer/CubemapRenderer.h>
#include <starlight/rendering/renderer/ModelRenderer.h>
#include <starlight/scene/SceneManager.h>

namespace starl::application {
class Application;
}

namespace starl::application::context {

struct ApplicationContextResource {
    friend class starl::application::Application;

    asset::AssetManager& assetManager;
    std::shared_ptr<scene::SceneManager> sceneManager;
    platform::window::Viewport viewport;

private:
    ApplicationContextResource(asset::AssetManager& assetManager,
        std::shared_ptr<scene::SceneManager> sceneManager, platform::window::Viewport viewport)
        : assetManager(assetManager)
        , sceneManager(std::move(sceneManager))
        , viewport(std::move(viewport)) {
    }
};
}