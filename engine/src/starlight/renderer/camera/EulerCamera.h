#pragma once

#include "Camera.h"

#include "starlight/core/math/Core.h"

namespace sl {

class EulerCamera : public Camera {
public:
    struct Properties {
        Vec3f target;
        float radius;
        Vec2u32 viewportSize;
    };

    explicit EulerCamera(const Properties& props);

    Mat4f getViewMatrix() const override;
    Mat4f getProjectionMatrix() const override;
    Vec3f getPosition() const override;

    void update(float deltaTime) override;

    void onScroll(float offset);

    void onViewportResize(Vec2u32 viewportSize) override;

private:
    void processInput(const float speed);
    void truncateCoefficients();
    void recalculateVectors();
    void updateViewMatrix();

    Vec3f m_target;
    float m_radius;

    Vec3f m_position;

    Vec3f m_front;
    Vec3f m_right;
    Vec3f m_up;

    float m_yaw;
    float m_pitch;

    Mat4f m_viewMatrix;
    Vec2u32 m_viewportSize;
};

}  // namespace sl