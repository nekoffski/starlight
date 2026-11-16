#pragma once

#include "Camera.hh"

namespace sl {

struct EulerCameraPose {
    Vec3f position{0.0f};
    f32 yawRadians{0.0f};
    f32 pitchRadians{0.0f};
    f32 rollRadians{0.0f};
};

struct PerspectiveProjection {
    f32 verticalFovRadians{glm::radians(60.0f)};
    f32 nearPlane{0.1f};
    f32 farPlane{1000.0f};
};

class EulerCamera final : public Camera {
   public:
    const EulerCameraPose& pose() const { return m_pose; }
    Result<void> setPose(EulerCameraPose pose);

    const PerspectiveProjection& projection() const { return m_projection; }
    Result<void> setProjection(PerspectiveProjection projection);

    Mat4f viewMatrix() const;
    Result<CameraMatrices> matrices(f32 aspectRatio) const override;

   private:
    EulerCameraPose m_pose{};
    PerspectiveProjection m_projection{};
};

}  // namespace sl
