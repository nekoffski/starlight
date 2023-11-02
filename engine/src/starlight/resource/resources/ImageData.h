#pragma once

#include <optional>
#include <functional>

#include "starlight/core/memory/Memory.hpp"
#include "starlight/resource/fwd.h"

namespace sl {

struct ImageData {
    unsigned char* pixels;
    uint32_t width;
    uint32_t height;
    uint8_t channels;
    bool isTransparent;
};

class STBImageData : public ImageData {
public:
    inline static const std::string baseTexturesPath =
      SL_ASSETS_PATH + std::string("/textures");

    ~STBImageData();

    STBImageData(STBImageData&& oth);
    STBImageData& operator=(STBImageData&& oth);

    static std::optional<STBImageData> load(
      const std::string& name, const std::string& imagesPath = baseTexturesPath
    );

private:
    explicit STBImageData(
      unsigned char* pixels, uint32_t width, uint32_t height, uint8_t channels,
      bool isTransparent
    );

    STBImageData(const STBImageData& oth)            = default;
    STBImageData& operator=(const STBImageData& oth) = default;

public:
};

}  // namespace sl
