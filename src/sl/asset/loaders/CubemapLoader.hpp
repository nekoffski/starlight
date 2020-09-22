#pragma once

#include "sl/asset/loaders/AssetLoader.h"
#include "sl/core/log/Logger.h"
#include "sl/core/path/PathManager.hpp"
#include "sl/platform/texture/Cubemap.h"

namespace sl::asset::loaders {

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

template <>
struct AssetLoader<Cubemap> {
    static std::shared_ptr<Cubemap> load(AssetLoaderArgs<Cubemap> args) {
        // clang-format off
        SL_DEBUG("loading cubemap: \n {}/{}/{}/{}/{}/{} ", args.top, args.bottom , args.right, args.left, args.front, args.back);
        return Cubemap::create({ 
            core::path::PathManager::createGlobalPath<Cubemap>(args.top),
            core::path::PathManager::createGlobalPath<Cubemap>(args.bottom),
            core::path::PathManager::createGlobalPath<Cubemap>(args.right),
            core::path::PathManager::createGlobalPath<Cubemap>(args.left),
            core::path::PathManager::createGlobalPath<Cubemap>(args.front),
            core::path::PathManager::createGlobalPath<Cubemap>(args.back) });
        // clang-format on
    }
};
}
