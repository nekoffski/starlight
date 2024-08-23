#include "ImageData.hh"

#include <stb.h>

#include "starlight/core/Log.hh"

namespace sl {

std::optional<ImageData> ImageData::loadFromFile(
  std::string_view name, Orientation orientation, std::string_view imagesPath
) {
    static constexpr int requiredChannels = 4;
    const auto path                       = fmt::format("{}/{}", imagesPath, name);

    LOG_TRACE("Loading image: '{}'", path);

    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(orientation == Orientation::flipped);

    const auto pixels =
      stbi_load(path.data(), &width, &height, &channels, requiredChannels);

    if (not pixels) {
        if (const auto error = stbi_failure_reason(); error)
            LOG_ERROR("Could not load '{}' - '{}'", path, error);
        return {};
    }

    ON_SCOPE_EXIT { stbi_image_free(pixels); };
    bool isTransparent = false;

    if (channels == requiredChannels) {
        static constexpr u8 opaqueAlpha = 255;

        for (u64 i = 3; i < width * height * channels; i += channels) {
            const auto alpha = *(pixels + i);
            if (alpha != opaqueAlpha) {
                isTransparent = true;
                break;
            }
        }
    } else {
        LOG_WARN(
          "Image '{}' has different channels count than required - {} != {}", path,
          requiredChannels, channels
        );
    }

    ImageData image;
    image.width    = static_cast<u32>(width);
    image.height   = static_cast<u32>(height);
    image.channels = static_cast<u32>(requiredChannels);

    const auto bufferSize = width * height * requiredChannels;

    image.pixels.resize(bufferSize, 0);
    std::memcpy(image.pixels.data(), pixels, bufferSize);

    LOG_TRACE(
      "Image loaded: width={}, height={}, channels={}", image.width, image.height,
      image.channels
    );

    return image;
}

}  // namespace sl
