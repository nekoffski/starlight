#pragma once

#include <kcore/Concepts.hh>

#include "starlight/core/Error.hh"
#include "starlight/math/Math.hh"

namespace sl {

struct CameraMatrices {
    Mat4f worldToView{1.0f};
    Mat4f viewToClip{1.0f};

    Mat4f worldToClip() const { return viewToClip * worldToView; }
};

class Camera : public kc::NonCopyable, public kc::NonMovable {
   public:
    virtual ~Camera() = default;

    virtual Result<CameraMatrices> matrices(f32 aspectRatio) const = 0;
};

}  // namespace sl
