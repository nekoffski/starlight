#include "EulerCamera.h"

#include <algorithm>

#include "sl/gui/GuiApi.h"
#include "sl/math/Matrix.hpp"
#include "sl/math/Utils.hpp"
#include "sl/core/Input.h"

#include "sl/core/Logger.h"

namespace sl::graphics::camera {

const float EulerCamera::MIN_PSI = -std::numbers::pi / 2.0f;
const float EulerCamera::MAX_PSI = std::numbers::pi / 2.0f;
const float EulerCamera::MIN_FI = 0.0f;
const float EulerCamera::MAX_FI = 2.0f * std::numbers::pi;

std::shared_ptr<EulerCamera> EulerCamera::create(math::Vec3 centre, float speed, float radius) {
    SL_INFO("creating instance, speed: {}, radius: {} ", speed, radius);
    return std::make_shared<EulerCamera>(centre, speed, radius);
}

EulerCamera::EulerCamera(math::Vec3 centre, float speed, float radius)
    : m_centre(centre)
    , m_speed(speed)
    , m_radius(radius)
    , m_fi(0.0f)
    , m_psi(0.0f) {
    calculateVectors();
}

void EulerCamera::update(float deltaTime) {
    float velocity = deltaTime * m_speed;

    if (m_direction & DIR_UP)
        m_psi += velocity;

    if (m_direction & DIR_RIGHT)
        m_fi += velocity;

    if (m_direction & DIR_DOWN)
        m_psi -= velocity;

    if (m_direction & DIR_LEFT)
        m_fi -= velocity;

    m_fi = math::circularRange(m_fi, MIN_FI, MAX_FI);

    // TODO: FIX
    m_psi = std::clamp(m_psi, MIN_PSI, MAX_PSI);

    calculateVectors();
}

void EulerCamera::calculateVectors() {
    m_position = {
        m_radius * std::cos(m_psi) * std::cos(m_fi),
        m_radius * std::sin(m_psi),
        m_radius * std::cos(m_psi) * std::sin(m_fi)
    };

    m_front = math::normalize(m_position - m_centre);
    m_right = math::cross(m_front, WORLD_UP);
    m_up = math::cross(m_right, m_front);
}

void EulerCamera::handleInput(std::shared_ptr<core::Input> input) {
    m_direction = DIR_NONE;

    if (input->isKeyPressed(STARL_KEY_UP))
        m_direction |= DIR_UP;

    if (input->isKeyPressed(STARL_KEY_RIGHT))
        m_direction |= DIR_RIGHT;

    if (input->isKeyPressed(STARL_KEY_DOWN))
        m_direction |= DIR_DOWN;

    if (input->isKeyPressed(STARL_KEY_LEFT))
        m_direction |= DIR_LEFT;
}

void EulerCamera::onGui(gui::GuiApi& gui) {
    if (gui.beginTreeNode("Euler camera")) {
        gui.sliderFloat("Fi", m_fi, MIN_FI, MAX_FI);
        gui.sliderFloat("Psi", m_psi, MIN_PSI, MAX_PSI);
        gui.sliderFloat("R", m_radius, 1.0f, 25.0f);
        gui.popTreeNode();
    }
}
}
