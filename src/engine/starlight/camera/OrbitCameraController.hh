#pragma once

#include "EulerCamera.hh"

namespace sl {

class Window;

class OrbitCameraController {
   public:
    explicit OrbitCameraController(Vec3f center = Vec3f{0.0f})
        : m_center(center) {}

    Result<void> update(
        EulerCamera& camera, const Window& window, f32 deltaSeconds
    );

   private:
    Vec3f m_center;
};

}  // namespace sl
