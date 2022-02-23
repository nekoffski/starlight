#pragma once

#include <memory>
#include <string>
#include <vector>

#include "sl/core/GameObject.h"
#include "sl/gfx/Image.h"

namespace sl::gfx {

constexpr unsigned int facesCount = 6;

using CubemapArgs = std::array<std::string, facesCount>;
using CubemapFaces = std::array<gfx::Image*, facesCount>;

class TextureManager;

class Cubemap : public core::GameObject {
    friend class TextureManager;

   public:
    struct Factory {
        virtual std::unique_ptr<Cubemap> create(const CubemapFaces&) = 0;
        virtual std::unique_ptr<Cubemap> create(unsigned int, unsigned int) = 0;
    };

    virtual ~Cubemap() = default;

    virtual unsigned int getBufferId() const = 0;

    virtual void bind(unsigned int index = s_currentTextureId) = 0;
    virtual void unbind() = 0;

    std::unique_ptr<Cubemap> clone();

    CubemapArgs getFacesPaths() const { return m_facesPaths; }

    std::string name;

    unsigned int width;
    unsigned int height;

   protected:
    inline static unsigned int s_currentTextureId = 0u;

   private:
    CubemapArgs m_facesPaths;
};
}  // namespace sl::gfx
