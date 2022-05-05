#pragma once

#include <string>

#include "sl/gpu/Image.h"
#include "sl/math/Size2D.h"

namespace sl::platform::stb {

class StbImage : public gpu::Image {
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

struct StbImageFactory : gpu::Image::Factory {
    std::unique_ptr<gpu::Image> create(const std::string& path, int desiredChannels = 0);
};
}  // namespace sl::platform::stb
