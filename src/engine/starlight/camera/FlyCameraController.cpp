#include "FlyCameraController.hh"

#include <cmath>

#include "starlight/window/Window.hh"

namespace sl {
namespace {

constexpr f32 movementSpeed = 3.0f;
constexpr f32 rollSpeed = 1.5f;
constexpr f32 mouseSensitivity = 0.003f;

f32 keyAxis(const Window& window, Key positive, Key negative) {
    return f32(window.isPressed(positive)) - f32(window.isPressed(negative));
}

Vec3f movementDelta(
    const EulerCamera& camera, const Window& window, f32 deltaSeconds
) {
    const Vec3f localMovement{
        keyAxis(window, Key::d, Key::a),
        0.0f,
        keyAxis(window, Key::s, Key::w),
    };
    const auto cameraToWorldRotation =
        glm::transpose(Mat3f{camera.viewMatrix()});
    auto worldMovement = cameraToWorldRotation * localMovement;
    worldMovement.y += keyAxis(window, Key::e, Key::q);

    const auto length = glm::length(worldMovement);
    if (length > 1.0f) {
        worldMovement /= length;
    }
    return worldMovement * movementSpeed * deltaSeconds;
}

}  // namespace

Vec2f FlyCameraController::mouseLookDelta(const Window& window) {
    if (not window.isPressed(MouseButton::right)) {
        m_previousMousePosition.reset();
        return Vec2f{0.0f};
    }

    const auto position = window.mousePosition();
    const auto delta = m_previousMousePosition
                           ? position - *m_previousMousePosition
                           : Vec2f{0.0f};
    m_previousMousePosition = position;
    return delta;
}

Result<void> FlyCameraController::update(
    EulerCamera& camera, const Window& window, f32 deltaSeconds
) {
    if (not std::isfinite(deltaSeconds) || deltaSeconds < 0) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Invalid camera update duration"
        );
    }
    const auto windowId = window.id();
    const auto focused = window.isFocused();
    if (m_window != windowId || not focused) {
        m_previousMousePosition.reset();
    }
    m_window = windowId;
    if (not focused) {
        return {};
    }

    auto pose = camera.pose();
    pose.position += movementDelta(camera, window, deltaSeconds);
    const auto rollInput = keyAxis(window, Key::z, Key::c);
    pose.rollRadians += rollInput * rollSpeed * deltaSeconds;

    const auto lookDelta = mouseLookDelta(window) * mouseSensitivity;
    pose.yawRadians -= lookDelta.x;
    pose.pitchRadians -= lookDelta.y;
    return camera.setPose(pose);
}

}  // namespace sl
