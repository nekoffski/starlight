#pragma once
#include "sl/gfx/Image.h"

#include <string>

#include <stb/stb.h>

#include "sl/core/Logger.h"
#include "sl/core/Errors.hpp"

namespace sl::platform::img {

class StbImage : public gfx::Image {
public:
    StbImage(const std::string& path, int desiredChannels = 0)
        : m_data(stbi_load(path.c_str(), &m_imageSize.width, &m_imageSize.height, &m_channels, desiredChannels)) {

        if (!m_data) {
            SL_ERROR("Could not load {}", path);
            throw core::ImageError{ core::ErrorCode::CouldNotLoadImage };
        }
    }

    ~StbImage() override {
        if (m_data)
            stbi_image_free(m_data);
    }

    unsigned char* getRaw() override {
        return m_data;
    }

    gfx::ImageSize getSize() override {
        return m_imageSize;
    }

    int getChannels() override {
        return m_channels;
    }

private:
    unsigned char* m_data{ nullptr };
    int m_channels;
    gfx::ImageSize m_imageSize;
};

struct StbImageFactory : gfx::Image::Factory {
    std::shared_ptr<gfx::Image> create(const std::string& path, int desiredChannels = 0) {
        return std::make_shared<StbImage>(path, desiredChannels);
    }
};
}
