#pragma once

#include <string>

#include "sl/gfx/Image.h"
#include "sl/math/Size2D.h"

namespace sl::platform::stb {

class StbImage : public gfx::Image {
public:
    StbImage(const std::string& path, int desiredChannels = 0);
    ~StbImage() override;

    unsigned char* getBuffer() const override;
    math::Size2D getSize() const override;
    int getChannelsCount() const override;

private:
    unsigned char* m_buffer;
    int m_channels;
    math::Size2D m_imageSize;
};

struct StbImageFactory : gfx::Image::Factory {
    std::unique_ptr<gfx::Image> create(const std::string& path, int desiredChannels = 0);
};
}
