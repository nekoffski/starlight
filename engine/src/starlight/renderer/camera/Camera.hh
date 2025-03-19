#pragma once

#include "starlight/core/Core.hh"
#include "starlight/core/math/Core.hh"
#include "starlight/event/EventProxy.hh"
#include "starlight/event/EventHandlerSentinel.hh"

namespace sl {

class Camera {
public:
    struct ProjectionProperties {
        f32 fov   = 45.0f;
        f32 nearZ = 0.1f;
        f32 farZ  = 1000.0f;

        inline static ProjectionProperties getDefault() {
            return ProjectionProperties{};
        }
    };

    explicit Camera(
      const Vec2<u32>& viewport, const Vec3<f32>& position,
      const ProjectionProperties& projectionProperties =
        ProjectionProperties::getDefault()
    );

    virtual void update(float deltaTime) = 0;

    const Vec3<f32>& getPosition() const;
    const Mat4<f32>& getViewMatrix() const;
    const Mat4<f32>& getInvViewMatrix() const;
    const Mat4<f32>& getProjectionMatrix() const;
    const Mat4<f32>& getInvProjectionMatrix() const;

protected:
    void setViewMatrix(const Mat4<f32>& viewMatrix);

    void calculateProjectionMatrix();

    Vec3<f32> m_position;
    EventHandlerSentinel m_eventSentinel;

private:
    Vec2<u32> m_viewportSize;
    Mat4<f32> m_projectionMatrix;
    Mat4<f32> m_invProjectionMatrix;
    Mat4<f32> m_viewMatrix;
    Mat4<f32> m_invViewMatrix;

    ProjectionProperties m_projectionProperties;
};

}  // namespace sl