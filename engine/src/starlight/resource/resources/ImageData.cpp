#include "ImageData.h"

#include <stb.h>

namespace sl {

STBImageData::STBImageData(
  unsigned char* pixels, uint32_t width, uint32_t height, uint8_t channels,
  bool isTransparent
) :
    ImageData{ pixels, width, height, channels, isTransparent } {}

STBImageData::~STBImageData() {
    if (pixels) stbi_image_free(pixels);
}

STBImageData::STBImageData(STBImageData&& oth) {
    *this      = oth;
    oth.pixels = nullptr;
}

STBImageData& STBImageData::operator=(STBImageData&& oth) {
    *this      = oth;
    oth.pixels = nullptr;
    return *this;
}

}  // namespace sl
