#pragma once

#include "starlight/renderer/gpu/Texture.h"
#include "starlight/core/math/Core.h"

namespace sl::ui {

struct ImageHandle {
    static std::unique_ptr<ImageHandle> createHandle(Texture* texture);
    virtual void show(const Vec2f& size, const Vec2f& minUV, const Vec2f& maxUV) = 0;
};

}  // namespace sl::ui
