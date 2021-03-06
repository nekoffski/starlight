#include "EulerCamera.h"

#include <algorithm>

#include "sl/core/Input.h"
#include "sl/gfx/ViewFrustum.h"
#include "sl/gui/GuiApi.h"
#include "sl/math/Matrix.hpp"
#include "sl/math/Utils.hpp"
#include "sl/utils/Globals.h"

#include "sl/core/Logger.h"

namespace sl::gfx::camera {

const float EulerCamera::minPsi = -std::numbers::pi / 2.0f;
const float EulerCamera::maxPsi = std::numbers::pi / 2.0f;
const float EulerCamera::minFi = 0.0f;
const float EulerCamera::maxFi = 2.0f * std::numbers::pi;

EulerCamera::EulerCamera(const ViewFrustum& viewFrustum, math::Vec3 center, float speed, float radius)
    : Camera(viewFrustum)
    , m_center(center)
    , m_speed(speed)
    , m_radius(radius)
    , m_fi(0.0f)
    , m_psi(0.0f) {
    calculateVectors();
    calculateProjectionMatrix();
}

void EulerCamera::update(float deltaTime, core::Input& input) {
    if (m_isInAnimation) {
        m_alpha += deltaTime * 2.0f;

        if (m_alpha >= 1.0f) {
            m_alpha = 0.0f;
            m_isInAnimation = false;
        }

    } else {
        float velocity = deltaTime * m_speed;

        if (input.isKeyPressed(STARL_KEY_UP))
            m_psi += velocity;

        if (input.isKeyPressed(STARL_KEY_RIGHT))
            m_fi += velocity;

        if (input.isKeyPressed(STARL_KEY_DOWN))
            m_psi -= velocity;

        if (input.isKeyPressed(STARL_KEY_LEFT))
            m_fi -= velocity;

        m_fi = math::circularRange(m_fi, minFi, maxFi);

        // TODO: FIX
        m_psi = std::clamp(m_psi, minPsi, maxPsi);

        m_isMouseMiddlePressed = input.isMouseButtonPressed(STARL_MOUSE_BUTTON_MIDDLE);

        if (m_isMouseMiddlePressed) {
            constexpr float mouseSpeed = 0.0015f;

            auto [deltaX, deltaY] = input.getMousePositonDelta();

            m_psi += deltaY * mouseSpeed;
            m_fi += deltaX * mouseSpeed;
        }

        auto scrollDelta = input.getScrollDelta();
        const float scrollDeltaSpeed = 5.0f;
        m_radius += scrollDelta * scrollDeltaSpeed;
    }

    calculateVectors();
}

void EulerCamera::calculateVectors() {
    if (m_isInAnimation)
        m_center = math::lerp(m_previousCenter, m_targetCenter, m_alpha);

    m_position = {
        m_radius * std::cos(m_psi) * std::cos(m_fi),
        m_radius * std::sin(m_psi),
        m_radius * std::cos(m_psi) * std::sin(m_fi)
    };

    m_position += m_center;

    m_front = math::normalize(-m_position + m_center);
    m_right = math::normalize(math::cross(m_front, worldUp));
    m_up = math::normalize(math::cross(m_right, m_front));
}

void EulerCamera::onGui(gui::GuiApi& gui) {
    if (gui.beginTreeNode("Euler camera")) {
        gui.sliderFloat("Fi", m_fi, minFi, maxFi);
        gui.sliderFloat("Psi", m_psi, minPsi, maxPsi);
        gui.sliderFloat("R", m_radius, 1.0f, 25.0f);
        gui.popTreeNode();
    }
}
}
