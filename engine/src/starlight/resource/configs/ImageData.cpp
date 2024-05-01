#include "ImageData.h"

#include <stb.h>

#include "starlight/core/Log.h"

namespace sl {

STBImageData::STBImageData(
  u8* pixels, u32 width, u32 height, u8 channels, bool isTransparent
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

std::optional<STBImageData> STBImageData::load(
  const Properties& props, const std::string& imagesPath
) {
    static constexpr int requiredChannels = 4;
    const auto fullPath = fmt::format("{}/{}", imagesPath, props.name);

    LOG_TRACE("Loading image: '{}'", fullPath);

    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(props.flip);

    const auto pixelsHandle =
      stbi_load(fullPath.c_str(), &width, &height, &channels, requiredChannels);

    if (not pixelsHandle) {
        if (const auto error = stbi_failure_reason(); error)
            LOG_ERROR("Could not load '{}' - '{}'", fullPath, error);
        return {};
    }

    bool isTransparent = false;

    if (channels == requiredChannels) {
        static constexpr u8 opaqueAlpha = 255;

        for (u64 i = 3; i < width * height * channels; i += channels) {
            const auto alpha = *(pixelsHandle + i);
            if (alpha != opaqueAlpha) {
                isTransparent = true;
                break;
            }
        }
    }

    if (channels != requiredChannels) {
        LOG_WARN(
          "Image '{}' has different channels count than required - {} != {}",
          props.name, requiredChannels, channels
        );
    }

    STBImageData image(
      pixelsHandle, static_cast<u32>(width), static_cast<u32>(height),
      static_cast<u8>(requiredChannels), isTransparent
    );

    LOG_TRACE(
      "Image loaded: width={}, height={}, channels={}", image.width, image.height,
      image.channels
    );

    return image;
}

}  // namespace sl
