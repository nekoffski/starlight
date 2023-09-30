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
    friend class ResourceLoader;

private:
public:
    ~STBImageData();

    STBImageData(STBImageData&& oth);
    STBImageData& operator=(STBImageData&& oth);

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
