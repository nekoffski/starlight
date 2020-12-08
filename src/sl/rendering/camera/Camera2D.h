#pragma once

#include "UserControllableCamera.h"
#include "sl/gui/GuiProxy.h"

#include <memory>

namespace sl::rendering::camera {

class Camera2D : public UserControllableCamera {
public:
    static std::shared_ptr<Camera2D> create(math::Vec3 position) {
        return std::make_shared<Camera2D>(std::move(position));
    }

    explicit Camera2D(math::Vec3 position) {}

    void update(float) override {
    }

    void handleInput(std::shared_ptr<platform::input::Input> input) override {
        const math::Vec3 up{ 0.0f, 1.0f, 0.0f };
        const math::Vec3 right{ 1.0f, 0.0f, 0.0f };

        if (input->isKeyPressed(STARL_KEY_UP)) {
            m_position += up * m_speed;
            m_centre += up * m_speed;
        }

        if (input->isKeyPressed(STARL_KEY_DOWN)) {
            m_position -= up * m_speed;
            m_centre -= up * m_speed;
        }

        if (input->isKeyPressed(STARL_KEY_RIGHT)) {
            m_position -= right * m_speed;
            m_centre -= right * m_speed;
        }

        if (input->isKeyPressed(STARL_KEY_LEFT)) {
            m_position += right * m_speed;
            m_centre += right * m_speed;
        }
    }

    void onGui(gui::GuiProxy& gui) override {
        if (gui.beginTreeNode("Camera")) {
            gui.sliderFloat("Camera speed", m_speed, 0.01f, 2.5f);
            gui.popTreeNode();
        }
    }

private:
    float m_speed;
    math::Vec3 m_position;
    math::Vec3 m_up;
    math::Vec3 m_front;
    math::Vec3 m_right;
    math::Vec3 m_centre;
};
}
