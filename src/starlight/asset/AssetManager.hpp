#pragma once

#include <memory>

#include <starlight/asset/loaders/ShaderLoader.hpp>

namespace {
template <typename T>
struct AssetLoaderDispatcher {
    inline static starl::asset::loaders::AssetLoader<T>* loader{ nullptr };
};

static starl::asset::loaders::ShaderLoader SHADER_LOADER;
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
}
