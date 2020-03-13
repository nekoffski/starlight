#include <starlight/platform/img/Image.h>
#include <starlight/platform/img/stb/StbImage.hpp>

namespace starl::platform::img {

std::shared_ptr<Image> Image::create(const std::string& path) {
    return std::make_shared<stb::StbImage>(path);
}
}