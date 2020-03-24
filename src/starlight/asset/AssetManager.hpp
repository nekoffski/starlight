#pragma once

#include <memory>

#include <starlight/asset/loaders/CubemapLoader.hpp>
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
}

namespace starl::asset {

class AssetManager {
public:
    template <typename T>
    std::shared_ptr<T> load(loaders::AssetLoaderArgs<T> args) {
        if (auto& loader = AssetLoaderDispatcher<T>::loader; loader != nullptr) {
            return loader->load(args);
        }
        throw AssetException(ErrorCode::LOADER_NOT_FOUND);
    }
};
}

namespace {

using namespace starl::asset;
using namespace starl::platform;

template <>
struct AssetLoaderDispatcher<shader::Shader> {
    inline static loaders::AssetLoader<shader::Shader>* loader{ &SHADER_LOADER };
};

template <>
struct AssetLoaderDispatcher<texture::Texture> {
    inline static loaders::AssetLoader<texture::Texture>* loader{ &TEXTURE_LOADER };
};

template <>
struct AssetLoaderDispatcher<texture::Cubemap> {
    inline static loaders::AssetLoader<texture::Cubemap>* loader{ &CUBEMAP_LOADER };
};
}
