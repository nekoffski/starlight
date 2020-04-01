#pragma once

#include <starlight/asset/loaders/AssetLoader.h>
#include <starlight/core/path/PathManager.hpp>

#include <starlight/core/log/Logger.h>
#include <starlight/platform/texture/Texture.h>

namespace starl::asset::loaders {

using platform::texture::Texture;

template <>
struct AssetLoaderArgs<Texture> {
    std::string path;
};

class TextureLoader : public AssetLoader<Texture> {
public:
    std::shared_ptr<Texture> load(AssetLoaderArgs<Texture> args) {
        logger->trace("loading texture: {}", args.path);
        return Texture::create(core::path::PathManager::createGlobalPath<Texture>(args.path));
    }

private:
    Logger logger{ starl::core::log::createLogger("TextureLoader") };
};
}
