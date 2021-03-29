#pragma once

#include "sl/asset/loaders/Loader.h"
#include "sl/core/Logger.h"
#include "sl/graphics/Cubemap.h"
#include "sl/utils/Globals.h"

namespace sl::asset::loaders {

using sl::graphics::Cubemap;

template <>
struct LoaderArgs<Cubemap> {
    explicit LoaderArgs(const std::string& right, const std::string& left, const std::string& top,
        const std::string& bottom, const std::string& front, const std::string& back)
        : right(right)
        , left(left)
        , top(top)
        , bottom(bottom)
        , front(front)
        , back(back) {
    }

    explicit LoaderArgs(const std::array<std::string, 6>& faces)
        : right(faces[0])
        , left(faces[1])
        , top(faces[2])
        , bottom(faces[3])
        , front(faces[4])
        , back(faces[5]) {
    }

    std::string right;
    std::string left;
    std::string top;
    std::string bottom;
    std::string front;
    std::string back;
};

template <>
struct Loader<Cubemap> {
    static std::shared_ptr<Cubemap> load(bool globalPath, LoaderArgs<Cubemap> args) {
        // clang-format off
		std::string prefix = "";
		if (not globalPath)
            prefix += GLOBALS().config.paths.cubemaps;

		SL_DEBUG("loading cubemap: \n {} - {}/{}/{}/{}/{}/{} ", prefix, args.right, args.left , args.top, args.bottom, args.front, args.back);
		return graphics::Cubemap::factory->create({ 
			prefix + args.right, prefix + args.left, prefix + args.top, prefix + args.bottom, prefix + args.front, prefix + args.back });
        // clang-format on
    }
};
}
