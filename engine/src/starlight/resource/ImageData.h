#pragma once

#include "Resource.h"
#include "starlight/core/memory/Memory.hpp"

#include <optional>

namespace sl {

// TODO: use custom allocator to store data on stack instead of heap

struct ImageData : public Resource {
    explicit ImageData(std::string_view fullPath, std::string_view name);
    ~ImageData();

    ImageData(const ImageData&)            = delete;
    ImageData(ImageData&&)                 = delete;
    ImageData& operator=(const ImageData&) = delete;
    ImageData& operator=(ImageData&&)      = delete;

    static UniqPtr<ImageData> create(std::string_view name);

    unsigned char* pixels;

    uint32_t width;
    uint32_t height;
    uint8_t channels;
    bool isTransparent;
};

}  // namespace sl