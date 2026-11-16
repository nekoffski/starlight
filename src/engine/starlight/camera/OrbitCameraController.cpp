#include "OrbitCameraController.hh"

#include <algorithm>
#include <cmath>

#include "starlight/window/Window.hh"

namespace sl {
namespace {

constexpr f32 angularSpeed = 1.5f;
constexpr f32 zoomSpeed = 3.0f;
constexpr f32 minDistance = 0.25f;
constexpr f32 maxDistance = 100.0f;
constexpr f32 maxElevation = glm::radians(89.0f);

struct OrbitPosition {
    f32 azimuth;
    f32 elevation;
    f32 distance;
};

f32 keyAxis(const Window& window, Key positive, Key negative) {
    return f32(window.isPressed(positive)) - f32(window.isPressed(negative));
}

Result<OrbitPosition> orbitFromPosition(Vec3f position, Vec3f center) {
    const auto offset = position - center;
    const auto distance = std::hypot(offset.x, offset.y, offset.z);
    if (not std::isfinite(distance) || distance == 0) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument,
            "Orbit camera must have a finite distance from its center"
        );
    }
    return OrbitPosition{
        std::atan2(offset.x, offset.z),
        std::atan2(offset.y, std::hypot(offset.x, offset.z)),
        distance,
    };
}

EulerCameraPose orbitPose(Vec3f center, const OrbitPosition& orbit) {
    const auto horizontalDistance = orbit.distance * std::cos(orbit.elevation);
    return {
        .position = center +
                    Vec3f{
                        horizontalDistance * std::sin(orbit.azimuth),
                        orbit.distance * std::sin(orbit.elevation),
                        horizontalDistance * std::cos(orbit.azimuth),
                    },
        .yawRadians = orbit.azimuth,
        .pitchRadians = -orbit.elevation,
    };
}

}  // namespace

Result<void> OrbitCameraController::update(
    EulerCamera& camera, const Window& window, f32 deltaSeconds
) {
    const bool validInput = std::isfinite(deltaSeconds) && deltaSeconds >= 0 &&
                            std::isfinite(m_center.x) &&
                            std::isfinite(m_center.y) &&
                            std::isfinite(m_center.z);
    if (not validInput) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument,
            "Invalid orbit center or update duration"
        );
    }
    if (not window.isFocused()) {
        return {};
    }

    auto orbit = orbitFromPosition(camera.pose().position, m_center);
    if (not orbit) {
        return Error::unexpected(orbit.error());
    }
    orbit->azimuth = std::remainder(
        orbit->azimuth +
            keyAxis(window, Key::d, Key::a) * angularSpeed * deltaSeconds,
        glm::two_pi<f32>()
    );
    orbit->elevation = std::clamp(
        orbit->elevation +
            keyAxis(window, Key::w, Key::s) * angularSpeed * deltaSeconds,
        -maxElevation, maxElevation
    );
    orbit->distance = std::clamp(
        orbit->distance +
            keyAxis(window, Key::q, Key::e) * zoomSpeed * deltaSeconds,
        minDistance, maxDistance
    );
    const auto pose = orbitPose(m_center, *orbit);
    if (pose.position == m_center) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument,
            "Orbit distance cannot be represented at this center"
        );
    }
    return camera.setPose(pose);
}

}  // namespace sl
