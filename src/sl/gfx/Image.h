#pragma once

#include <memory>

#include "sl/math/Size2D.h"

namespace sl::gfx {

class TextureManager;

class Image {
    friend class TextureManager;

   public:
    struct Factory {
        virtual std::unique_ptr<Image> create(const std::string& path, int desiredChannels = 0) = 0;
    };

    virtual ~Image() = default;

    virtual unsigned char* getBuffer() const = 0;
    virtual math::Size2D getSize() const     = 0;
    virtual int getChannelsCount() const     = 0;

    std::string getPath() const { return m_path; }

   private:
    std::string m_path;
};
}  // namespace sl::gfx
