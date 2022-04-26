#pragma once

#include "Camera.h"
#include "sl/gfx/ViewFrustum.h"
#include "sl/math/Utils.hpp"

namespace sl::cam {

class EulerCamera : public Camera {
   public:
    explicit EulerCamera(const gfx::ViewFrustum& viewFrustum, math::Vec3, float, float);

    void update(float) override;
    void onGui() override;

    const math::Mat4 getViewMatrix() const override {
        return math::lookAt(m_position, m_center, m_up);
    }

    void setCenter(const math::Vec3& center) {
        m_previousCenter = m_center;
        m_targetCenter   = center;

        m_isInAnimation = true;
    }

    void calculateVectors();

   private:
    bool m_isInAnimation = false;
    float m_alpha        = 0.0f;

    math::Vec3 m_center;
    math::Vec3 m_previousCenter;
    math::Vec3 m_targetCenter;

    float m_radius;
    float m_speed;

    float m_fi;
    float m_psi;

    bool m_isMouseMiddlePressed = false;

    static const float minPsi;
    static const float maxPsi;
    static const float minFi;
    static const float maxFi;
};
}  // namespace sl::cam
