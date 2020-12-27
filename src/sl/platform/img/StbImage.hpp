#pragma once
#include "sl/graphics/Image.h"

#include <string>

#include <stb/stb.h>

#include "sl/core/Logger.h"
#include "sl/core/error/Errors.hpp"

namespace sl::platform::img {

class StbImage : public graphics::Image {
public:
    StbImage(const std::string& path, int desiredChannels = 0)
        : m_data(stbi_load(path.c_str(), &m_imageSize.width, &m_imageSize.height, &m_channels, desiredChannels)) {

        if (!m_data) {
            SL_ERROR("Could not load {}", path);
            throw core::error::ImageError{ core::error::ErrorCode::CouldNotLoadImage };
        }
    }

    ~StbImage() override {
        if (m_data)
            stbi_image_free(m_data);
    }

    unsigned char* getRaw() override {
        return m_data;
    }

    graphics::ImageSize getSize() override {
        return m_imageSize;
    }

    int getChannels() override {
        return m_channels;
    }

private:
    unsigned char* m_data{ nullptr };
    int m_channels;
    graphics::ImageSize m_imageSize;
};

struct StbImageFactory : graphics::Image::Factory {
    std::shared_ptr<graphics::Image> create(const std::string& path, int desiredChannels = 0) {
        return std::make_shared<StbImage>(path, desiredChannels);
    }
};
}
