#pragma once

#include <imgui/imgui.h>

#include <ImGuizmo.h>

#include "sl/math/Matrix.hpp"
#include "sl/math/Vector.hpp"

#include "sl/gfx/ViewFrustum.h"

namespace sl::gui {

enum class GizmoOp {
    translate,
    rotate,
    scale
};

enum class GizmoSpace {
    local,
    world
};

void setupGizmo(const gfx::ViewFrustum::Viewport& viewport);

bool isUsingGizmo();

void manipulateGizmo(math::Mat4& viewMatrix, math::Mat4& projectionMatrix,
    math::Mat4& transformation, const GizmoOp op, const GizmoSpace system);

}