#pragma once

#include "Camera.h"
#include "sl/gfx/ViewFrustum.h"
#include "sl/math/Utils.hpp"

namespace sl::gfx::camera {

class EulerCamera : public Camera {
public:
    explicit EulerCamera(const ViewFrustum& viewFrustum, math::Vec3, float, float);

    void update(float) override;
    void onGui() override;

    const math::Mat4 getViewMatrix() override {
        return math::lookAt(m_position, m_center, m_up);
    }

    void setCenter(const math::Vec3& center) {
        m_previousCenter = m_center;
        m_targetCenter = center;

        m_isInAnimation = true;
    }

    void calculateVectors();

    void calculateProjectionMatrix() override {
        float aspect = static_cast<float>(viewFrustum.viewport.width) / viewFrustum.viewport.height;
        m_projectionMatrix = math::perspective(viewFrustum.fieldOfView, aspect, viewFrustum.nearZ, viewFrustum.farZ);
    }

private:
    bool m_isInAnimation = false;
    float m_alpha = 0.0f;

    Direction m_direction;

    math::Vec3 m_center;
    math::Vec3 m_previousCenter;
    math::Vec3 m_targetCenter;

    float m_radius;
    float m_speed;

    float m_fi;
    float m_psi;

    math::Vec3 m_position;
    math::Vec3 m_up;
    math::Vec3 m_front;
    math::Vec3 m_right;

    bool m_isMouseMiddlePressed = false;

    static const float minPsi;
    static const float maxPsi;
    static const float minFi;
    static const float maxFi;
};
}
