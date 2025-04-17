#pragma once

#include "starlight/renderer/gpu/Texture.hh"
#include "starlight/ui/Core.hh"

namespace sl {

struct ImageHandle {
    static kstd::UniquePtr<ImageHandle> createHandle(Texture* texture);
    virtual void show(
      const Vec2<f32>& size, const Vec2<f32>& minUV, const Vec2<f32>& maxUV
    ) = 0;
};

}  // namespace sl
