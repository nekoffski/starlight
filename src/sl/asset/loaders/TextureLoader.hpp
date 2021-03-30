#pragma once

#include "sl/asset/loaders/Loader.h"

#include "sl/core/Logger.h"
#include "sl/gfx/Texture.h"
#include "sl/utils/Globals.h"

namespace sl::asset::loaders {

using sl::gfx::Texture;

template <>
struct LoaderArgs<Texture> {
    std::string path;
};

template <>
struct Loader<Texture> {
    static std::shared_ptr<Texture> load(bool globalPath, LoaderArgs<Texture> args) {
        std::string path = "";
        if (not globalPath)
            path += GLOBALS().config.paths.textures;
        path += args.path;

        SL_DEBUG("loading texture: {}", path);
        return gfx::Texture::factory->create(path);
    }
};
}
