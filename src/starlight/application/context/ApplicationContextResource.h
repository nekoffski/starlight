#pragma once

#include <starlight/asset/AssetManager.hpp>
#include <starlight/rendering/renderer/CubemapRenderer.h>
#include <starlight/rendering/renderer/ModelRenderer.h>

namespace starl::application {
class Application;
}

namespace starl::application::context {

struct ApplicationContextResource {
    friend class starl::application::Application;

    std::shared_ptr<rendering::renderer::CubemapRenderer> cubemapRenderer;
    std::shared_ptr<rendering::renderer::ModelRenderer> modelRenderer;
    asset::AssetManager& assetManager;

private:
    ApplicationContextResource(std::shared_ptr<rendering::renderer::CubemapRenderer> cubemapRenderer,
        std::shared_ptr<rendering::renderer::ModelRenderer> modelRenderer,
        asset::AssetManager& assetManager)
        : cubemapRenderer(std::move(cubemapRenderer))
        , modelRenderer(std::move(modelRenderer))
        , assetManager(assetManager) {
    }
};
}