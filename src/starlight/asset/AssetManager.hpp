#pragma once

#include <memory>

#include "starlight/asset/Error.h"
#include "starlight/asset/loaders/CubemapLoader.hpp"
#include "starlight/asset/loaders/ModelLoaderWrapper.hpp"
#include "starlight/asset/loaders/ShaderLoader.hpp"
#include "starlight/asset/loaders/TextureLoader.hpp"
#include "starlight/gui/Window.h"

namespace {

template <typename T>
struct AssetLoaderDispatcher {
    inline static sl::asset::loaders::AssetLoader<T>* loader{ nullptr };
};

static sl::asset::loaders::ShaderLoader SHADER_LOADER;
static sl::asset::loaders::TextureLoader TEXTURE_LOADER;
static sl::asset::loaders::CubemapLoader CUBEMAP_LOADER;
static sl::asset::loaders::ModelLoaderWrapper MODEL_LOADER_WRAPPER;
}

namespace sl::asset {

class AssetManager {
public:
    template <typename T>
    static std::shared_ptr<T> load(loaders::AssetLoaderArgs<T> args) {
        if (auto& loader = AssetLoaderDispatcher<T>::loader; loader != nullptr)
            return loader->load(args);
        throw AssetException(ErrorCode::LOADER_NOT_FOUND);
    }

    void onGUI(gui::Window& window) {
    }
};
}

namespace {

using namespace sl::asset;

template <>
struct AssetLoaderDispatcher<sl::platform::shader::Shader> {
    inline static loaders::AssetLoader<sl::platform::shader::Shader>* loader{ &SHADER_LOADER };
};

template <>
struct AssetLoaderDispatcher<sl::platform::texture::Texture> {
    inline static loaders::AssetLoader<sl::platform::texture::Texture>* loader{ &TEXTURE_LOADER };
};

template <>
struct AssetLoaderDispatcher<sl::platform::texture::Cubemap> {
    inline static loaders::AssetLoader<sl::platform::texture::Cubemap>* loader{ &CUBEMAP_LOADER };
};

template <>
struct AssetLoaderDispatcher<sl::geometry::Model> {
    inline static loaders::AssetLoader<sl::geometry::Model>* loader{ &MODEL_LOADER_WRAPPER };
};
}
