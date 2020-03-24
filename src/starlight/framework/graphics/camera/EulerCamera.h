#pragma once

#include <starlight/framework/graphics/camera/UserControllableCamera.h>

namespace starl::framework::graphics::camera {

class EulerCamera : public UserControllableCamera {
public:
    static std::shared_ptr<EulerCamera> create(math::Vec3, float, float);

    explicit EulerCamera(math::Vec3, float, float);

    void update(float) override;

    const math::Vec3& getPosition() override;
    const math::Vec3& getUp() override;
    const math::Vec3& getFront() override;
    const math::Vec3& getRight() override;

    const math::Mat4 getViewMatrix() override;

    void handleInput(std::unique_ptr<platform::input::Input>&) override;

private:
    void calculateVectors();

    Direction m_direction;

    math::Vec3 m_centre;
    float m_radius;
    float m_speed;

    float m_fi;
    float m_psi;

    math::Vec3 m_position;
    math::Vec3 m_up;
    math::Vec3 m_front;
    math::Vec3 m_right;

    static const float MIN_PSI;
    static const float MAX_PSI;
    static const float MIN_FI;
    static const float MAX_FI;
};
}
