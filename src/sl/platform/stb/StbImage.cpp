#include "StbImage.h"

#include <stb/stb.h>

#include "sl/core/Errors.hpp"
#include "sl/core/Logger.h"

namespace sl::platform::stb {

StbImage::StbImage(const std::string& path, int desiredChannels)
    : m_buffer(stbi_load(path.c_str(), &m_imageSize.width, &m_imageSize.height, &m_channels, desiredChannels)) {

    if (!m_buffer) {
        SL_ERROR("Could not load {}", path);
        throw core::ImageError { core::ErrorCode::CouldNotLoadImage };
    }
}

StbImage::~StbImage() {
    if (m_buffer)
        stbi_image_free(m_buffer);
}

unsigned char* StbImage::getBuffer() const {
    return m_buffer;
}

math::Size2D StbImage::getSize() const {
    return m_imageSize;
}

int StbImage::getChannelsCount() const {
    return m_channels;
}

std::unique_ptr<gfx::Image> StbImageFactory::create(const std::string& path, int desiredChannels) {
    return std::make_unique<StbImage>(path, desiredChannels);
}
}
