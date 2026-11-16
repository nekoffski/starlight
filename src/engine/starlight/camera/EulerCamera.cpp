#include "EulerCamera.hh"

#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>

namespace sl {
namespace {

Mat4f makeViewMatrix(const EulerCameraPose& pose) {
    // Right-handed, looking down -Z: viewToWorld = T * Ry * Rx * Rz.
    auto rotation = glm::rotate(Mat4f{1.0f}, pose.yawRadians, Vec3f{0, 1, 0});
    rotation = glm::rotate(rotation, pose.pitchRadians, Vec3f{1, 0, 0});
    rotation = glm::rotate(rotation, pose.rollRadians, Vec3f{0, 0, 1});
    return glm::transpose(rotation) *
           glm::translate(Mat4f{1.0f}, -pose.position);
}

Mat4f makeProjection(const PerspectiveProjection& projection, f32 aspect) {
    return glm::perspectiveRH_ZO(
        projection.verticalFovRadians, aspect, projection.nearPlane,
        projection.farPlane
    );
}

bool isValidPose(const EulerCameraPose& pose) {
    const bool finitePose =
        std::isfinite(pose.position.x) && std::isfinite(pose.position.y) &&
        std::isfinite(pose.position.z) && std::isfinite(pose.yawRadians) &&
        std::isfinite(pose.pitchRadians) && std::isfinite(pose.rollRadians);
    return finitePose && isFinite(makeViewMatrix(pose));
}

bool isValidProjection(const PerspectiveProjection& projection) {
    const bool validProjection =
        std::isfinite(projection.verticalFovRadians) &&
        std::isfinite(projection.nearPlane) &&
        std::isfinite(projection.farPlane) &&
        projection.verticalFovRadians > 0 &&
        projection.verticalFovRadians < glm::pi<f32>() &&
        projection.nearPlane > 0 && projection.farPlane > projection.nearPlane;
    return validProjection && isFinite(makeProjection(projection, 1.0f));
}

}  // namespace

Result<void> EulerCamera::setPose(EulerCameraPose pose) {
    if (not isValidPose(pose)) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Invalid camera pose"
        );
    }
    m_pose = pose;
    return {};
}

Result<void> EulerCamera::setProjection(PerspectiveProjection projection) {
    if (not isValidProjection(projection)) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Invalid camera projection"
        );
    }
    m_projection = projection;
    return {};
}

Mat4f EulerCamera::viewMatrix() const { return makeViewMatrix(m_pose); }

Result<CameraMatrices> EulerCamera::matrices(f32 aspectRatio) const {
    if (not std::isfinite(aspectRatio) || aspectRatio <= 0) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Invalid camera aspect ratio"
        );
    }
    CameraMatrices result{
        viewMatrix(), makeProjection(m_projection, aspectRatio)
    };
    if (not isFinite(result.viewToClip)) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Camera projection is not finite"
        );
    }
    return result;
}

}  // namespace sl
