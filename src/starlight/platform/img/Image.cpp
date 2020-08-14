#include "Image.h"

#include "starlight/core/log/Logger.h"
#include "starlight/platform/img/stb/StbImage.hpp"

namespace sl::platform::img {

std::shared_ptr<Image> Image::create(const std::string& path) {
    SL_DEBUG("loading image: {} using StbImage", path);
    return std::make_shared<stb::StbImage>(path);
}
}