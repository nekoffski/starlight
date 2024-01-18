#pragma once

#include <optional>
#include <functional>

#include <starlight/core/Core.h>
#include "starlight/core/memory/Memory.hpp"
#include "starlight/resource/fwd.h"

namespace sl {

struct ImageData {
    u8* pixels;
    u32 width;
    u32 height;
    u8 channels;
    bool isTransparent;
};

class STBImageData : public ImageData {
public:
    struct Properties {
        std::string name;
        bool flip;
    };

    inline static const std::string baseTexturesPath =
      SL_ASSETS_PATH + std::string("/textures");

    ~STBImageData();

    STBImageData(STBImageData&& oth);
    STBImageData& operator=(STBImageData&& oth);

    static std::optional<STBImageData> load(
      const Properties& props, const std::string& imagesPath = baseTexturesPath
    );

private:
    explicit STBImageData(
      u8* pixels, u32 width, u32 height, u8 channels, bool isTransparent
    );

    STBImageData(const STBImageData& oth)            = default;
    STBImageData& operator=(const STBImageData& oth) = default;

public:
};

}  // namespace sl
