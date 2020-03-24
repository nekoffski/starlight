#include <starlight/platform/img/Image.h>
#include <starlight/platform/img/stb/StbImage.hpp>

#include <starlight/core/log/Logger.h>

static auto logger = starl::core::log::createLogger("Image");

namespace starl::platform::img {

std::shared_ptr<Image> Image::create(const std::string& path) {
    logger->trace("loading image: {} using StbImage", path);
    return std::make_shared<stb::StbImage>(path);
}
}