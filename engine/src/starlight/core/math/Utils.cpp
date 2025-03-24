#include "Utils.hh"

#include <glm/gtx/matrix_decompose.hpp>

namespace sl {

Vec3<f32> deproject(
  const Vec2<f32>& mouseCords, const Vec2<u32>& viewport,
  const Mat4<f32>& invProjectionMatrix, const Mat4<f32>& invViewMatrix
) {
    Vec3<f32> rayNdc{
        (2.0f * mouseCords.x) / viewport.x - 1.0f,
        1.0f - (2.0f * mouseCords.y) / viewport.y,
        1.0f,
    };

    Vec4<f32> rayClip{ rayNdc.x, rayNdc.y, -1.0f, 1.0f };
    Vec4<f32> rayEye = invProjectionMatrix * rayClip;

    rayEye.z = -1.0f;
    rayEye.w = 0.0f;

    return math::normalize(Vec3<f32>(invViewMatrix * rayEye));
}

MatrixComponents decomposeTransformation(const Mat4<f32>& matrix) {
    MatrixComponents components;

    math::decompose(
      matrix, components.scale, components.rotation, components.translation,
      components.skew, components.perspective
    );

    components.rotation = math::conjugate(components.rotation);
    components.euler    = math::eulerAngles(components.rotation);

    return components;
}

}  // namespace sl