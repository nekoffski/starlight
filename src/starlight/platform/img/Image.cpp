#include <starlight/platform/img/Image.h>
#include <starlight/platform/img/stb/StbImage.hpp>

#include <starlight/core/log/Logger.h>

namespace starl::platform::img {

std::shared_ptr<Image> Image::create(const std::string& path) {
    LOG(DEBUG) << "loading image: " << path << " using StbImage";
    return std::make_shared<stb::StbImage>(path);
}
}