#pragma once

#include "Camera.hh"

#include "starlight/math/Core.hh"

namespace sl {

class FirstPersonCamera : public Camera {
public:
    struct Properties {
        Vec3<f32> position;
        Vec2<u32> viewportSize;
    };

    explicit FirstPersonCamera(const Properties& props);

    void update(float deltaTime) override;

private:
    void processInput(const float speed);

    Vec3<f32> m_front;
    Vec3<f32> m_up;
    Vec3<f32> m_right;

    float m_yaw;
    float m_pitch;
    float m_speed;
};

}  // namespace sl