#pragma once

#include "UserControllableCamera.h"

namespace sl::rendering::camera {

class EulerCamera : public UserControllableCamera {
public:
    static std::shared_ptr<EulerCamera> create(math::Vec3, float, float);

    explicit EulerCamera(math::Vec3, float, float);

    void update(float) override;
    void handleInput(std::shared_ptr<platform::input::Input>) override;
    void onGui(gui::GuiProxy&) override;

    const math::Mat4 getViewMatrix() override {
        return math::lookAt(m_position, m_centre, m_up);
    }

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
