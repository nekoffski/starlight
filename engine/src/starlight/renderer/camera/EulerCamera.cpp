#include "EulerCamera.hh"

#include "starlight/window/Input.hh"
#include "starlight/window/Keys.hh"
#include "starlight/window/Window.hh"

namespace sl {

EulerCamera::EulerCamera(const Properties& props)
    : Camera(props.viewportSize, Vec3<f32>{ 0.0f })
    , m_target(props.target)
    , m_radius(props.radius)
    , m_yaw(90.0f)
    , m_pitch(90.0f) {
    recalculateVectors();
    updateViewMatrix();

    m_eventSentinel.add<sl::ScrollEvent>([&](auto& event) { onScroll(event.offset); }
    );
}

void EulerCamera::update(float deltaTime) {
    static constexpr float speed = 50.0f;

    if (m_animation) {
        m_animation->update();
        if (m_animation->done()) m_animation.reset();
    }

    processInput(speed * deltaTime);
    truncateCoefficients();
    recalculateVectors();
    updateViewMatrix();
}

void EulerCamera::onScroll(float offset) {
    static constexpr f32 scrollSpeed = 0.25f;
    m_radius -= scrollSpeed * offset;
}

void EulerCamera::lookAt(const Vec3<f32>& target, u32 steps) {
    if (steps == 1u)
        m_target = target;
    else
        m_animation.emplace(*this, target, steps);
}

void EulerCamera::processInput(const float speed) {
    auto& input = Input::get();

    if (input.isKeyPressed(SL_KEY_W)) m_pitch -= speed;
    if (input.isKeyPressed(SL_KEY_S)) m_pitch += speed;
    if (input.isKeyPressed(SL_KEY_D)) m_yaw -= speed;
    if (input.isKeyPressed(SL_KEY_A)) m_yaw += speed;

    if (input.isKeyPressed(SL_KEY_1)) m_radius += speed / 5.0f;
    if (input.isKeyPressed(SL_KEY_2)) m_radius -= speed / 5.0f;

    if (input.isMouseButtonPressed(SL_MOUSE_BUTTON_2)) {
        input.hideCursor();

        const float scaledSpeed = speed / 10.0f;
        const auto delta        = scaledSpeed * input.getMousePositionDelta();

        m_pitch -= delta.y;
        m_yaw += delta.x;
    } else {
        input.showCursor();
    }
}

void EulerCamera::truncateCoefficients() {
    if (m_yaw >= 360.0f) m_yaw = 0.0f;
    if (m_yaw < 0.0f) m_yaw = 360.0f;

    if (m_pitch > 170.0f) m_pitch = 170.0f;
    if (m_pitch < 10.0f) m_pitch = 10.0f;

    static constexpr float maxRadius = 100.0f;

    if (m_radius < 0.0f) m_radius = 0.0f;
    if (m_radius > maxRadius) m_radius = maxRadius;
}

void EulerCamera::recalculateVectors() {
    const static Vec3<f32> worldUp = Vec3<f32>{ 0.0f, 1.0f, 0.0f };

    const auto pitchRadians = glm::radians(m_pitch);
    const auto yawRadians   = glm::radians(m_yaw);

    // clang-format off
    m_position = m_target +  m_radius * Vec3<f32> {
        std::sin(pitchRadians) * std::cos(yawRadians),
        std::cos(pitchRadians),
        std::sin(pitchRadians) * std::sin(yawRadians)
    };
    // clang-format on

    m_front = glm::normalize(m_target - m_position);
    m_right = glm::cross(m_front, worldUp);
    m_up    = glm::cross(m_right, m_front);
}

void EulerCamera::updateViewMatrix() {
    setViewMatrix(glm::lookAt(m_position, m_target, m_up));
}

EulerCamera::Properties EulerCamera::Properties::createDefault() {
    return EulerCamera::Properties{
        .target       = sl::Vec3<sl::f32>{ 0.0f },
        .radius       = 5.0f,
        .viewportSize = Window::get().getFramebufferSize(),
    };
}

EulerCamera::Animation::Animation(
  EulerCamera& camera, const Vec3<f32>& target, u32 steps
)
    : m_camera(camera)
    , m_done(false)
    , m_target(target)
    , m_step((m_target - camera.m_target) / static_cast<f32>(steps)) {}

void EulerCamera::Animation::update() {
    static constexpr f32 delta = 0.005f;

    auto& cameraTarget = m_camera.m_target;
    cameraTarget += m_step;

    if (almostEquals(cameraTarget, m_target, delta)) {
        cameraTarget = m_target;
        m_done       = true;
    }
}

bool EulerCamera::Animation::done() const { return m_done; }

}  // namespace sl