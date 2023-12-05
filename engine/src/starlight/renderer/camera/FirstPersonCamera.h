#pragma once

#include "Camera.h"

#include "starlight/core/math/Glm.h"
#include "starlight/core/window/WindowManager.h"

namespace sl {

class FirstPersonCamera : public Camera {
public:
    struct Properties {
        Vec3f position;
        u32 viewportWidth;
        u32 viewportHeight;
    };

    explicit FirstPersonCamera(const Properties& props);

    Mat4f getViewMatrix() const override;
    Mat4f getProjectionMatrix() const override;
    Vec3f getPosition() const override;

    void update(float deltaTime) override;

    void onViewportResize(u32 w, u32 h) override;

private:
    void processInput(const float speed);

    Vec3f m_position;
    Vec3f m_front;
    Vec3f m_up;
    Vec3f m_right;

    float m_yaw;
    float m_pitch;
    float m_speed;

    u32 m_viewportWidth;
    u32 m_viewportHeight;
};

}  // namespace sl