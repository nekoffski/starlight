#pragma once

#include <memory>

#include <starlight/asset/Error.h>
#include <starlight/asset/loaders/CubemapLoader.hpp>
#include <starlight/asset/loaders/ModelLoaderWrapper.hpp>
#include <starlight/asset/loaders/ShaderLoader.hpp>
#include <starlight/asset/loaders/TextureLoader.hpp>

namespace {

template <typename T>
struct AssetLoaderDispatcher {
    inline static starl::asset::loaders::AssetLoader<T>* loader{ nullptr };
};

static starl::asset::loaders::ShaderLoader SHADER_LOADER;
static starl::asset::loaders::TextureLoader TEXTURE_LOADER;
static starl::asset::loaders::CubemapLoader CUBEMAP_LOADER;
static starl::asset::loaders::ModelLoaderWrapper MODEL_LOADER_WRAPPER;
}

namespace starl::asset {

class AssetManager {
public:
    template <typename T>
    std::shared_ptr<T> load(loaders::AssetLoaderArgs<T> args) {
        if (auto& loader = AssetLoaderDispatcher<T>::loader; loader != nullptr)
            return loader->load(args);
        throw AssetException(ErrorCode::LOADER_NOT_FOUND);
    }
};
}

namespace {

using namespace starl::asset;

template <>
struct AssetLoaderDispatcher<starl::platform::shader::Shader> {
    inline static loaders::AssetLoader<starl::platform::shader::Shader>* loader{ &SHADER_LOADER };
};

template <>
struct AssetLoaderDispatcher<starl::platform::texture::Texture> {
    inline static loaders::AssetLoader<starl::platform::texture::Texture>* loader{ &TEXTURE_LOADER };
};

template <>
struct AssetLoaderDispatcher<starl::platform::texture::Cubemap> {
    inline static loaders::AssetLoader<starl::platform::texture::Cubemap>* loader{ &CUBEMAP_LOADER };
};

template <>
struct AssetLoaderDispatcher<starl::geometry::Model> {
    inline static loaders::AssetLoader<starl::geometry::Model>* loader{ &MODEL_LOADER_WRAPPER };
};
}
