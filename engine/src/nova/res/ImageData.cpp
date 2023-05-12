#include "ImageData.h"

#include <fmt/core.h>
#include <stb.h>
#include <kc/core/Log.h>

#include "Utils.h"

namespace nova::res {

ImageData::~ImageData() {
    if (pixels) stbi_image_free(pixels);
}

core::UniqPtr<ImageData> ImageData::create(std::string_view name) {
    static constexpr int requiredChannels = 4;
    const auto fullPath = fmt::format("{}/textures/{}", getAssetsBasePath(), name);

    LOG_TRACE("Loading image: '{}'", fullPath);

    auto imageData = core::createUniqPtr<ImageData>(fullPath, name);

    int width;
    int height;
    int channels;

    imageData->pixels = stbi_load(fullPath.c_str(), &width, &height, &channels, requiredChannels);

    if (not imageData->pixels) {
        if (const auto error = stbi_failure_reason(); error)
            LOG_ERROR("Could not load '{}' - '{}'", fullPath, error);
        return nullptr;
    }

    if (channels != requiredChannels) {
        LOG_WARN(
            "Image '{}' has different channels count than required - {} != {}", name,
            requiredChannels, channels
        );
    }
    imageData->channels      = static_cast<uint8_t>(requiredChannels);
    imageData->width         = static_cast<uint32_t>(width);
    imageData->height        = static_cast<uint32_t>(height);
    imageData->isTransparent = true;  // TODO: implement

    LOG_TRACE(
        "Image loaded: width={}, height={}, channels={}", imageData->width, imageData->height,
        imageData->channels
    );

    return imageData;
}

ImageData::ImageData(std::string_view fullPath, std::string_view name)
    : Resource(fullPath, name)
    , pixels(nullptr)
    , width(0)
    , height(0)
    , channels(0)
    , isTransparent(false) {}

}  // namespace nova::res
