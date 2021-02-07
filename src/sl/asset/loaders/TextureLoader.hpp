#pragma once

#include "sl/asset/loaders/Loader.h"

#include "sl/core/Logger.h"
#include "sl/core/PathManager.hpp"
#include "sl/graphics/Texture.h"

namespace sl::asset::loaders {

using sl::graphics::Texture;

template <>
struct LoaderArgs<Texture> {
    std::string path;
};

template <>
struct Loader<Texture> {
    static std::shared_ptr<Texture> load(bool globalPath, LoaderArgs<Texture> args) {
        std::string path = "";
        if (not globalPath)
            path += core::PathManager::get<Texture>();
        path += args.path;

        SL_DEBUG("loading texture: {}", path);
        return graphics::Texture::factory->create(path);
    }
};
}
