#pragma once

#include <memory>
#include <string>
#include <vector>

#include "sl/core/GameObject.h"
#include "sl/core/Output.h"
#include "sl/gfx/Image.h"

namespace sl::gfx {

constexpr unsigned int facesCount = 6;
using CubemapArgs = std::array<std::string, facesCount>;
using CubemapFaces = std::array<gfx::Image*, facesCount>;

class Cubemap : public core::GameObject {
public:
    struct Factory {
        virtual std::unique_ptr<Cubemap> create(const CubemapFaces&) = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    static std::unique_ptr<Cubemap> load(const CubemapArgs& paths, const std::string& name);
    static void loadAsync(const CubemapArgs& paths, const std::string& name, std::unique_ptr<core::Output<Cubemap>> output);

    virtual ~Cubemap() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    CubemapArgs getFacesPaths() const {
        return m_facesPaths;
    }

    std::string name;

private:
    CubemapArgs m_facesPaths;
};
}
