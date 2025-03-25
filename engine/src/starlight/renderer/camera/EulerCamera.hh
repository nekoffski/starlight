#pragma once

#include <optional>

#include "Camera.hh"

#include "starlight/core/math/Core.hh"

namespace sl {

class EulerCamera : public Camera {
    class Animation {
    public:
        explicit Animation(EulerCamera& camera, const Vec3<f32>& target);

        void update();
        bool done() const;

    private:
        EulerCamera& m_camera;
        bool m_done;
        Vec3<f32> m_target;
        Vec3<f32> m_step;
    };

    friend class Animation;

public:
    struct Properties {
        Vec3<f32> target;
        float radius;
        Vec2<u32> viewportSize;

        static Properties createDefault();
    };

    explicit EulerCamera(const Properties& props = Properties::createDefault());

    void update(float deltaTime) override;
    void onScroll(float offset);

    void lookAt(const Vec3<f32>& target, f32 time = 0.0f) override;

private:
    void processInput(const float speed);
    void truncateCoefficients();
    void recalculateVectors();
    void updateViewMatrix();

    Vec3<f32> m_target;
    float m_radius;

    Vec3<f32> m_front;
    Vec3<f32> m_right;
    Vec3<f32> m_up;

    float m_yaw;
    float m_pitch;

    std::optional<Animation> m_animation;
};

}  // namespace sl