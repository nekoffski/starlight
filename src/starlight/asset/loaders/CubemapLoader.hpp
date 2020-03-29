#pragma once

#include <starlight/asset/PathManager.hpp>
#include <starlight/asset/loaders/AssetLoader.h>

#include <starlight/platform/texture/Cubemap.h>

#include <starlight/core/log/Logger.h>

namespace starl::asset::loaders {

using platform::texture::Cubemap;

template <>
struct AssetLoaderArgs<Cubemap> {
    std::string top;
    std::string bottom;
    std::string right;
    std::string left;
    std::string front;
    std::string back;
};

class CubemapLoader : public AssetLoader<Cubemap> {
public:
    std::shared_ptr<Cubemap> load(AssetLoaderArgs<Cubemap> args) {
        // clang-format off
        logger->trace("loading cubemap {}, {}, {}, {}, {}, {}", args.top, args.bottom, args.right, args.left, args.front, args.back);
        return Cubemap::create({ 
            PathManager::createGlobalPath<Cubemap>(args.top),
            PathManager::createGlobalPath<Cubemap>(args.bottom),
            PathManager::createGlobalPath<Cubemap>(args.right),
            PathManager::createGlobalPath<Cubemap>(args.left),
            PathManager::createGlobalPath<Cubemap>(args.front),
            PathManager::createGlobalPath<Cubemap>(args.back) });
        // clang-format on
    }

private:
    Logger logger{ starl::core::log::createLogger("CubemapLoader") };
};
}
