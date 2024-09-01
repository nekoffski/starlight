#pragma once

#include "Camera.hh"

#include "starlight/core/math/Core.hh"

namespace sl {

class FirstPersonCamera : public Camera {
public:
    struct Properties {
        Vec3<f32> position;
        Vec2<u32> viewportSize;
    };

    explicit FirstPersonCamera(const Properties& props);

    Mat4<f32> getViewMatrix() const override;
    Mat4<f32> getProjectionMatrix() const override;
    Vec3<f32> getPosition() const override;

    void update(float deltaTime) override;

    void onViewportResize(Vec2<u32> viewportSize) override;

private:
    void processInput(const float speed);

    Vec3<f32> m_position;
    Vec3<f32> m_front;
    Vec3<f32> m_up;
    Vec3<f32> m_right;

    float m_yaw;
    float m_pitch;
    float m_speed;

    Vec2<u32> m_viewportSize;
};

}  // namespace sl