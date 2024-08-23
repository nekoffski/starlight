#pragma once

#include <optional>
#include <functional>

#include "starlight/core/Core.hh"
#include "starlight/core/memory/Memory.hh"

namespace sl {

struct ImageData {
    inline static const std::string baseImagesPath =
      SL_ASSETS_PATH + std::string("/textures");

    enum class Orientation { flipped, nonFlipped };

    static std::optional<ImageData> loadFromFile(
      std::string_view path, Orientation orientation = Orientation::flipped,
      std::string_view imagesPath = baseImagesPath
    );

    std::vector<u8> pixels;
    u32 width;
    u32 height;
    u8 channels;
    bool isTransparent;
};

}  // namespace sl
