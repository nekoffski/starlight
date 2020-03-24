#pragma once

#include <starlight/asset/PathManager.hpp>
#include <starlight/asset/loaders/AssetLoader.h>

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
        return Texture::create(PathManager::createGlobalPath<Texture>(args.path));
    }
};
}
