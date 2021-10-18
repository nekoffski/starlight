#include "Gizmo.h"

#include <unordered_map>

#include "sl/math/Utils.hpp"

namespace sl::gui {

void setupGizmo(const sl::gfx::ViewFrustum::Viewport& viewport) {
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetRect(viewport.beginX, 0, viewport.width, viewport.height);
}

bool isUsingGizmo() {
    return ImGuizmo::IsUsing();
}

void manipulateGizmo(math::Mat4& viewMatrix, math::Mat4& projectionMatrix,
    math::Mat4& transformation, const GizmoOp op, const GizmoSpace space) {

    using namespace ImGuizmo;

    // clang-format off
    static const std::unordered_map<GizmoOp, ImGuizmo::OPERATION> operationLookupTable = {
        { GizmoOp::translate, OPERATION::TRANSLATE },
        { GizmoOp::rotate,    OPERATION::ROTATE },
        { GizmoOp::scale,     OPERATION::SCALE }
    };

    static const std::unordered_map<GizmoSpace, MODE> spaceLookupTable = {
        { GizmoSpace::local, MODE::LOCAL },
        { GizmoSpace::world, MODE::WORLD },
    };
    // clang-format on

    ImGuizmo::Manipulate(math::valuePtr(viewMatrix), math::valuePtr(projectionMatrix),
        operationLookupTable.at(op), spaceLookupTable.at(space), math::valuePtr(transformation));
}
}