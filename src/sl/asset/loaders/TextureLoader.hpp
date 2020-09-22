#pragma once

#include "sl/asset/loaders/AssetLoader.h"

#include "sl/core/log/Logger.h"
#include "sl/core/path/PathManager.hpp"
#include "sl/platform/texture/Texture.h"

namespace sl::asset::loaders {

using platform::texture::Texture;

template <>
struct AssetLoaderArgs<Texture> {
    std::string path;
};

template <>
struct AssetLoader<Texture> {
    static std::shared_ptr<Texture> load(AssetLoaderArgs<Texture> args) {
        SL_DEBUG("loading texture: {}", args.path);
        return Texture::create(core::path::PathManager::createGlobalPath<Texture>(args.path));
    }
};
}
