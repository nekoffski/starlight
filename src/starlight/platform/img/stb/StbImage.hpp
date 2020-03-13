#pragma once

#include <string>

#include <stb/stb.h>

#include <starlight/platform/Error.h>
#include <starlight/platform/img/Image.h>

namespace starl::platform::img::stb {

class StbImage : public Image {
public:
    StbImage(const std::string& path, int desiredChannels = 0)
        : m_data(stbi_load(path.c_str(), &m_imageSize.first, &m_imageSize.second, &m_channels, desiredChannels)) {

        if (!m_data) {
            throw PlatformException(ErrorCode::COULD_NOT_LOAD_IMAGE);
        }
    }

    ~StbImage() override {
        if (m_data) {
            stbi_image_free(m_data);
        }
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
};
}