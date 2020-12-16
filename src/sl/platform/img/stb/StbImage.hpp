#pragma once
#include "sl/platform/img/Image.h"

#include <string>

#include <stb/stb.h>

#include "sl/core/Logger.h"
#include "sl/platform/Error.h"

namespace sl::platform::img::stb {

class StbImage : public Image {
public:
    StbImage(const std::string& path, int desiredChannels = 0)
        : m_data(stbi_load(path.c_str(), &m_imageSize.width, &m_imageSize.height, &m_channels, desiredChannels)) {

        if (!m_data) {
            SL_ERROR("Could not load {}", path);
            throw PlatformException(ErrorCode::COULD_NOT_LOAD_IMAGE);
        }
    }

    ~StbImage() override {
        if (m_data)
            stbi_image_free(m_data);
    }

    unsigned char* getRaw() override {
        return m_data;
    }

    ImageSize getSize() override {
        return m_imageSize;
    }

    int getChannels() override {
        return m_channels;
    }

private:
    unsigned char* m_data{ nullptr };
    int m_channels;
    ImageSize m_imageSize;
    int w;
    int h;
};
}