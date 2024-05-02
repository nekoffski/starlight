#pragma once

#include "starlight/renderer/gpu/Texture.h"
#include "starlight/core/math/Core.h"

namespace sl::ui {

struct ImageHandle {
    static std::unique_ptr<ImageHandle> createHandle(Texture* texture);
    virtual void show(
      const Vec2<f32>& size, const Vec2<f32>& minUV, const Vec2<f32>& maxUV
    ) = 0;
};

}  // namespace sl::ui
