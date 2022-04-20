#pragma once

// clang-format off
#include <imgui/imgui.h>
#include <ImGuizmo.h>
// clang-format on

#include "sl/gfx/ViewFrustum.h"
#include "sl/math/Matrix.hpp"
#include "sl/math/Vector.hpp"

namespace sl::gui {

enum class GizmoOp { translate, rotate, scale };

enum class GizmoSpace { local, world };

void setupGizmo(const gfx::Viewport& viewport);

bool isUsingGizmo();

void manipulateGizmo(math::Mat4& viewMatrix, math::Mat4& projectionMatrix,
                     math::Mat4& transformation, const GizmoOp op, const GizmoSpace system);

}  // namespace sl::gui