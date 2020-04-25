#pragma once

#include <starlight/asset/AssetManager.hpp>
#include <starlight/rendering/renderer/CubemapRenderer.h>
#include <starlight/rendering/renderer/ModelRenderer.h>
#include <starlight/scene/SceneManager.h>

namespace starl::application {
class Application;
}

namespace starl::application::context {

struct ApplicationContextResource {
    friend class starl::application::Application;

    std::shared_ptr<rendering::renderer::CubemapRenderer> cubemapRenderer;
    std::shared_ptr<rendering::renderer::ModelRenderer> modelRenderer;
    asset::AssetManager& assetManager;
    std::shared_ptr<scene::SceneManager> sceneManager;

private:
    ApplicationContextResource(std::shared_ptr<rendering::renderer::CubemapRenderer> cubemapRenderer,
        std::shared_ptr<rendering::renderer::ModelRenderer> modelRenderer,
        asset::AssetManager& assetManager,
        std::shared_ptr<scene::SceneManager> sceneManager)
        : cubemapRenderer(std::move(cubemapRenderer))
        , modelRenderer(std::move(modelRenderer))
        , assetManager(assetManager)
        , sceneManager(std::move(sceneManager)) {
    }
};
}