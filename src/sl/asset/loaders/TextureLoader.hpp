#pragma once

#include "sl/asset/loaders/AssetLoader.h"

#include "sl/core/Logger.h"
#include "sl/core/PathManager.hpp"
#include "sl/platform/texture/Texture.h"

namespace sl::asset::loaders {

using platform::texture::Texture;

template <>
struct AssetLoaderArgs<Texture> {
    std::string path;
};

template <>
struct AssetLoader<Texture> {
    static std::shared_ptr<Texture> load(bool globalPath, AssetLoaderArgs<Texture> args) {
        std::string path = "";
        if (not globalPath)
            path += core::PathManager::get<Texture>();
        path += args.path;

        SL_DEBUG("loading texture: {}", path);
        return Texture::create(path);
    }
};
}
