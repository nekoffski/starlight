#pragma once

#include "sl/asset/loaders/Loader.h"
#include "sl/core/Logger.h"
#include "sl/core/PathManager.hpp"
#include "sl/graphics/Cubemap.h"

namespace sl::asset::loaders {

using sl::graphics::Cubemap;

template <>
struct LoaderArgs<Cubemap> {
    explicit LoaderArgs(const std::string& top, const std::string& bottom, const std::string& right,
        const std::string& left, const std::string& front, const std::string& back)
        : top(top)
        , bottom(bottom)
        , right(right)
        , left(left)
        , front(front)
        , back(back) {
    }

    explicit LoaderArgs(const std::array<std::string, 6>& faces)
        : top(faces[0])
        , bottom(faces[1])
        , right(faces[2])
        , left(faces[3])
        , front(faces[4])
        , back(faces[5]) {
    }

    std::string top;
    std::string bottom;
    std::string right;
    std::string left;
    std::string front;
    std::string back;
};

template <>
struct Loader<Cubemap> {
    static std::shared_ptr<Cubemap> load(bool globalPath, LoaderArgs<Cubemap> args) {
        // clang-format off
		std::string prefix = "";
		if (not globalPath)
            prefix += core::PathManager::get<Cubemap>();

		SL_DEBUG("loading cubemap: \n {} - {}/{}/{}/{}/{}/{} ", prefix, args.top, args.bottom , args.right, args.left, args.front, args.back);
		return graphics::Cubemap::factory->create({ 
			prefix + args.right, prefix + args.left, prefix + args.top, prefix + args.bottom, prefix + args.front, prefix + args.back });
        // clang-format on
    }
};
}
