#pragma once

#include "Camera.h"

#include "nova/math/Glm.h"

namespace nova::gfx {

class EulerCamera : public Camera {
   public:
    struct Properties {
        math::Vec3f target;
        float radius;
    };

    explicit EulerCamera(const Properties& props);

    math::Mat4f getViewMatrix() const override;
    math::Mat4f getProjectionMatrix() const override;
    math::Vec3f getPosition() const override;

    void update(float deltaTime) override;

   private:
    void processInput(const float speed);
    void truncateCoefficients();
    void recalculateVectors();
    void updateViewMatrix();

    math::Vec3f m_target;
    float m_radius;

    math::Vec3f m_position;

    math::Vec3f m_front;
    math::Vec3f m_right;
    math::Vec3f m_up;

    float m_yaw;
    float m_pitch;

    math::Mat4f m_viewMatrix;
};

}  // namespace nova::gfx