#pragma once

#include "UserControllableCamera.h"

namespace sl::graphics::camera {

class EulerCamera : public UserControllableCamera {
public:
    static std::shared_ptr<EulerCamera> create(math::Vec3, float, float);

    explicit EulerCamera(math::Vec3, float, float);

    void update(float) override;
    void handleInput(std::shared_ptr<core::Input>) override;
    void onGui(gui::GuiApi&) override;

    const math::Mat4 getViewMatrix() override {
        return math::lookAt(m_position, m_center, m_up);
    }

    void setCenter(const math::Vec3& center) {
        m_previousCenter = m_center;
        m_previousPosition = m_position;
        m_targetCenter = center;

        m_isInAnimation = true;
    }

    void calculateVectors();

private:
    bool m_isInAnimation = false;
    float m_alpha = 0.0f;

    Direction m_direction;

    math::Vec3 m_center;
    math::Vec3 m_previousCenter;
    math::Vec3 m_targetCenter;
    math::Vec3 m_previousPosition;

    float m_radius;
    float m_speed;

    float m_fi;
    float m_psi;

    math::Vec3 m_position;
    math::Vec3 m_up;
    math::Vec3 m_front;
    math::Vec3 m_right;

    static const float minPsi;
    static const float maxPsi;
    static const float minFi;
    static const float maxFi;
};
}
