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
    static std::shared_ptr<Cubemap> load(bool globalPath, AssetLoaderArgs<Cubemap> args) {
        // clang-format off
		std::string prefix = "";
		if (not globalPath)
            prefix += core::path::PathManager::get<Cubemap>();

		SL_DEBUG("loading cubemap: \n {} - {}/{}/{}/{}/{}/{} ", prefix, args.top, args.bottom , args.right, args.left, args.front, args.back);
        return Cubemap::create({ 
            prefix + args.right, prefix + args.left, prefix + args.top, prefix + args.bottom, prefix + args.front, prefix + args.back });
        // clang-format on
    }
};
}
