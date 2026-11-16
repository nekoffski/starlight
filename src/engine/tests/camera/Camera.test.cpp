#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <type_traits>

#include "starlight/camera/FlyCameraController.hh"
#include "starlight/camera/OrbitCameraController.hh"
#include "starlight/window/Window.hh"

namespace {

using namespace sl;

static_assert(not std::is_copy_constructible_v<EulerCamera>);
static_assert(not std::is_copy_assignable_v<EulerCamera>);
static_assert(not std::is_move_constructible_v<EulerCamera>);
static_assert(not std::is_move_assignable_v<EulerCamera>);
static_assert(std::is_copy_constructible_v<CameraMatrices>);

constexpr f32 tolerance = 0.0001f;

void expectVector(Vec4f actual, Vec4f expected) {
    for (int i = 0; i < actual.length(); ++i) {
        EXPECT_NEAR(actual[i], expected[i], tolerance);
    }
}

TEST(EulerCamera, ProducesIndependentViewAndProjectionValues) {
    EulerCamera camera;
    const Camera& interface = camera;
    const auto original = interface.matrices(1.0f);
    ASSERT_TRUE(original);
    expectVector(original->worldToView * Vec4f{1, 2, 3, 1}, Vec4f{1, 2, 3, 1});
    ASSERT_TRUE(camera.setPose({.position = {2, 3, 4}}));
    expectVector(camera.viewMatrix() * Vec4f{2, 3, 4, 1}, Vec4f{0, 0, 0, 1});
    expectVector(original->worldToView * Vec4f{1, 2, 3, 1}, Vec4f{1, 2, 3, 1});
}

TEST(EulerCamera, UsesRightHandedYawPitchRollInSpecifiedOrder) {
    EulerCamera camera;
    const auto quarterTurn = glm::half_pi<f32>();
    ASSERT_TRUE(camera.setPose({.yawRadians = quarterTurn}));
    expectVector(camera.viewMatrix() * Vec4f{-1, 0, 0, 1}, Vec4f{0, 0, -1, 1});
    ASSERT_TRUE(camera.setPose({.pitchRadians = quarterTurn}));
    expectVector(camera.viewMatrix() * Vec4f{0, 1, 0, 1}, Vec4f{0, 0, -1, 1});
    ASSERT_TRUE(camera.setPose({.rollRadians = quarterTurn}));
    expectVector(camera.viewMatrix() * Vec4f{0, 1, 0, 1}, Vec4f{1, 0, 0, 1});
    ASSERT_TRUE(camera.setPose({
        .yawRadians = quarterTurn,
        .pitchRadians = quarterTurn,
        .rollRadians = quarterTurn,
    }));
    expectVector(camera.viewMatrix() * Vec4f{1, 2, 3, 1}, Vec4f{1, 3, -2, 1});
}

TEST(EulerCamera, MapsPerspectiveDepthToZeroOneAndUsesViewAspect) {
    EulerCamera camera;
    ASSERT_TRUE(camera.setProjection({.nearPlane = 1.0f, .farPlane = 10.0f}));
    const auto square = camera.matrices(1.0f);
    const auto wide = camera.matrices(2.0f);
    ASSERT_TRUE(square);
    ASSERT_TRUE(wide);
    const auto nearClip = square->worldToClip() * Vec4f{0, 0, -1, 1};
    const auto farClip = square->worldToClip() * Vec4f{0, 0, -10, 1};
    EXPECT_NEAR(nearClip.z / nearClip.w, 0.0f, tolerance);
    EXPECT_NEAR(farClip.z / farClip.w, 1.0f, tolerance);
    EXPECT_NEAR(
        wide->viewToClip[0][0], square->viewToClip[0][0] / 2, tolerance
    );
    EXPECT_EQ(wide->viewToClip[1][1], square->viewToClip[1][1]);
}

TEST(EulerCamera, RejectsInvalidInputsWithoutChangingState) {
    EulerCamera camera;
    const auto nan = std::numeric_limits<f32>::quiet_NaN();
    const auto infinity = std::numeric_limits<f32>::infinity();
    EXPECT_FALSE(camera.setPose({.position = {nan, 0, 0}}));
    EXPECT_FALSE(camera.setPose({.pitchRadians = infinity}));
    EXPECT_EQ(camera.pose().position, Vec3f{0});
    EXPECT_EQ(camera.pose().pitchRadians, 0.0f);
    const auto original = camera.projection();
    for (auto projection : {
             PerspectiveProjection{.verticalFovRadians = 0},
             PerspectiveProjection{.verticalFovRadians = glm::pi<f32>()},
             PerspectiveProjection{.nearPlane = 0},
             PerspectiveProjection{.nearPlane = 2, .farPlane = 1},
             PerspectiveProjection{.farPlane = infinity},
             PerspectiveProjection{.verticalFovRadians = nan},
         }) {
        EXPECT_FALSE(camera.setProjection(projection));
        EXPECT_EQ(
            camera.projection().verticalFovRadians, original.verticalFovRadians
        );
        EXPECT_EQ(camera.projection().nearPlane, original.nearPlane);
        EXPECT_EQ(camera.projection().farPlane, original.farPlane);
    }
    for (auto aspect : {0.0f, -1.0f, nan, infinity}) {
        const auto matrices = camera.matrices(aspect);
        ASSERT_FALSE(matrices);
        EXPECT_EQ(matrices.error().code(), kc::ErrorCode::invalidArgument);
    }
    EXPECT_FALSE(camera.matrices(std::numeric_limits<f32>::denorm_min()));
    const auto largest = std::numeric_limits<f32>::max();
    EXPECT_FALSE(camera.setPose(
        {.position = Vec3f{largest}, .yawRadians = glm::quarter_pi<f32>()}
    ));
    EXPECT_EQ(camera.pose().position, Vec3f{0});
}

class TestWindow : public Window {
   public:
    void pollEvents() override {}
    WindowId id() const override { return windowId; }
    bool isPressed(Key key) const override {
        return std::ranges::find(keys, key) != keys.end();
    }
    bool isPressed(MouseButton button) const override {
        return dragging && button == MouseButton::right;
    }
    bool isFocused() const override { return focused; }
    Vec2u drawableSize() const override { return {800, 600}; }
    Vec2f mousePosition() const override { return cursor; }

    WindowId windowId{1};
    std::vector<Key> keys;
    bool focused{true};
    bool dragging{false};
    Vec2f cursor{0.0f};
};

TEST(FlyCameraController, MovesInCameraSpaceAndNormalizesDiagonalSpeed) {
    EulerCamera camera;
    FlyCameraController controller;
    TestWindow window;
    window.keys = {Key::w};
    ASSERT_TRUE(controller.update(camera, window, 0.5f));
    const auto distance = glm::length(camera.pose().position);
    EXPECT_GT(distance, 0);
    EXPECT_LT(camera.pose().position.z, 0);
    ASSERT_TRUE(camera.setPose({.yawRadians = glm::half_pi<f32>()}));
    ASSERT_TRUE(controller.update(camera, window, 0.5f));
    EXPECT_NEAR(camera.pose().position.x, -distance, tolerance);
    EXPECT_NEAR(camera.pose().position.z, 0, tolerance);

    ASSERT_TRUE(camera.setPose({}));
    window.keys = {Key::w, Key::d, Key::e};
    ASSERT_TRUE(controller.update(camera, window, 0.5f));
    EXPECT_NEAR(glm::length(camera.pose().position), distance, tolerance);
    window.keys = {Key::z};
    ASSERT_TRUE(controller.update(camera, window, 0.5f));
    EXPECT_GT(camera.pose().rollRadians, 0);
}

TEST(FlyCameraController, ResetsMouseLookAcrossDragFocusAndWindowChanges) {
    EulerCamera camera;
    FlyCameraController controller;
    TestWindow window;
    window.dragging = true;
    window.cursor = {100, 100};
    ASSERT_TRUE(controller.update(camera, window, 0));
    EXPECT_EQ(camera.pose().yawRadians, 0);
    window.cursor = {110, 90};
    ASSERT_TRUE(controller.update(camera, window, 0));
    EXPECT_LT(camera.pose().yawRadians, 0);
    EXPECT_GT(camera.pose().pitchRadians, 0);
    const auto pose = camera.pose();

    window.focused = false;
    window.keys = {Key::w};
    ASSERT_TRUE(controller.update(camera, window, 1));
    EXPECT_EQ(camera.pose().position, pose.position);
    window.focused = true;
    window.keys.clear();
    window.cursor = {300, 300};
    ASSERT_TRUE(controller.update(camera, window, 0));
    EXPECT_EQ(camera.pose().yawRadians, pose.yawRadians);
    window.dragging = false;
    ASSERT_TRUE(controller.update(camera, window, 0));
    window.dragging = true;
    window.cursor = {400, 400};
    ASSERT_TRUE(controller.update(camera, window, 0));
    EXPECT_EQ(camera.pose().yawRadians, pose.yawRadians);
    window.windowId = WindowId{2};
    window.cursor = {900, 900};
    ASSERT_TRUE(controller.update(camera, window, 0));
    EXPECT_EQ(camera.pose().yawRadians, pose.yawRadians);
}

TEST(FlyCameraController, RejectsInvalidTimeWithoutMoving) {
    EulerCamera camera;
    FlyCameraController controller;
    TestWindow window;
    window.keys = {Key::w};
    for (auto delta :
         {-1.0f, std::numeric_limits<f32>::infinity(),
          std::numeric_limits<f32>::quiet_NaN()}) {
        EXPECT_FALSE(controller.update(camera, window, delta));
        EXPECT_EQ(camera.pose().position, Vec3f{0});
    }
}

TEST(
    OrbitCameraController, KeepsConfiguredCenterInViewWithoutChangingProjection
) {
    const Vec3f center{2, -3, 4};
    EulerCamera camera;
    ASSERT_TRUE(camera.setPose({
        .position = center + Vec3f{0, 0, 5},
        .yawRadians = 1.0f,
        .pitchRadians = 0.3f,
        .rollRadians = 0.2f,
    }));
    ASSERT_TRUE(camera.setProjection({.verticalFovRadians = 1.0f}));
    OrbitCameraController controller{center};
    TestWindow window;
    ASSERT_TRUE(controller.update(camera, window, 0));
    EXPECT_EQ(camera.pose().position, (center + Vec3f{0, 0, 5}));
    EXPECT_EQ(camera.pose().rollRadians, 0);
    EXPECT_EQ(camera.projection().verticalFovRadians, 1.0f);
    expectVector(camera.viewMatrix() * Vec4f{center, 1}, Vec4f{0, 0, -5, 1});
}

TEST(OrbitCameraController, RotatesOnSphereAndKeepsLookingAtCenter) {
    EulerCamera camera;
    ASSERT_TRUE(camera.setPose({.position = {0, 0, 5}}));
    OrbitCameraController controller;
    TestWindow window;
    window.keys = {Key::w, Key::d};
    ASSERT_TRUE(controller.update(camera, window, 0.25f));
    EXPECT_GT(camera.pose().position.x, 0);
    EXPECT_GT(camera.pose().position.y, 0);
    EXPECT_NEAR(glm::length(camera.pose().position), 5.0f, tolerance);
    expectVector(camera.viewMatrix() * Vec4f{0, 0, 0, 1}, Vec4f{0, 0, -5, 1});
    window.keys = {Key::s, Key::a};
    ASSERT_TRUE(controller.update(camera, window, 0.25f));
    expectVector(Vec4f{camera.pose().position, 1}, Vec4f{0, 0, 5, 1});
}

TEST(OrbitCameraController, ZoomsWithDistanceLimits) {
    EulerCamera camera;
    ASSERT_TRUE(camera.setPose({.position = {0, 0, 2}}));
    OrbitCameraController controller;
    TestWindow window;
    window.keys = {Key::q};
    ASSERT_TRUE(controller.update(camera, window, 0.25f));
    EXPECT_GT(camera.pose().position.z, 2.0f);
    window.keys = {Key::e};
    ASSERT_TRUE(controller.update(camera, window, 0.25f));
    EXPECT_NEAR(camera.pose().position.z, 2.0f, tolerance);
    ASSERT_TRUE(controller.update(camera, window, 100.0f));
    const auto closest = camera.pose().position;
    EXPECT_GT(closest.z, 0);
    EXPECT_LT(closest.z, 2.0f);
    ASSERT_TRUE(controller.update(camera, window, 100.0f));
    EXPECT_EQ(camera.pose().position, closest);
    window.keys = {Key::q};
    ASSERT_TRUE(controller.update(camera, window, 100.0f));
    const auto farthest = camera.pose().position;
    EXPECT_GT(farthest.z, 2.0f);
    ASSERT_TRUE(controller.update(camera, window, 100.0f));
    EXPECT_EQ(camera.pose().position, farthest);
}

TEST(OrbitCameraController, StopsBeforePolesWithoutFlipping) {
    EulerCamera camera;
    ASSERT_TRUE(camera.setPose({.position = {0, 0, 2}}));
    OrbitCameraController controller;
    TestWindow window;
    window.keys = {Key::w};
    ASSERT_TRUE(controller.update(camera, window, 100.0f));
    EXPECT_GT(camera.pose().pitchRadians, -glm::half_pi<f32>());
    EXPECT_LT(camera.pose().pitchRadians, 0);
    EXPECT_TRUE(isFinite(camera.viewMatrix()));
    expectVector(camera.viewMatrix() * Vec4f{0, 0, 0, 1}, Vec4f{0, 0, -2, 1});
    window.keys = {Key::s};
    ASSERT_TRUE(controller.update(camera, window, 100.0f));
    EXPECT_LT(camera.pose().pitchRadians, glm::half_pi<f32>());
    EXPECT_GT(camera.pose().pitchRadians, 0);
    EXPECT_TRUE(isFinite(camera.viewMatrix()));
}

TEST(OrbitCameraController, RejectsInvalidInputAndLeavesUnfocusedCameraAlone) {
    EulerCamera camera;
    ASSERT_TRUE(camera.setPose({.position = {0, 0, 2}}));
    OrbitCameraController controller;
    TestWindow window;
    window.keys = {Key::w, Key::d, Key::e};
    window.focused = false;
    ASSERT_TRUE(controller.update(camera, window, 1));
    EXPECT_EQ(camera.pose().position, (Vec3f{0, 0, 2}));
    EXPECT_EQ(camera.pose().pitchRadians, 0);
    window.focused = true;
    for (auto delta :
         {-1.0f, std::numeric_limits<f32>::infinity(),
          std::numeric_limits<f32>::quiet_NaN()}) {
        EXPECT_FALSE(controller.update(camera, window, delta));
        EXPECT_EQ(camera.pose().position, (Vec3f{0, 0, 2}));
    }
    OrbitCameraController badCenter{
        Vec3f{std::numeric_limits<f32>::infinity()}
    };
    EXPECT_FALSE(badCenter.update(camera, window, 1));
    ASSERT_TRUE(camera.setPose({}));
    EXPECT_FALSE(controller.update(camera, window, 1));
    OrbitCameraController unrepresentableCenter{Vec3f{1e20f}};
    EXPECT_FALSE(unrepresentableCenter.update(camera, window, 0));
    EXPECT_EQ(camera.pose().position, Vec3f{0});
}

}  // namespace
