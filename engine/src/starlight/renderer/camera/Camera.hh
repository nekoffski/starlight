#pragma once

#include "starlight/core/Core.hh"
#include "starlight/core/math/Core.hh"
#include "starlight/core/event/EventHandlerSentinel.hh"

namespace sl {

// TODO: move some common stuff to the base class
class Camera {
public:
    explicit Camera();

    virtual Mat4<f32> getViewMatrix() const       = 0;
    virtual Mat4<f32> getProjectionMatrix() const = 0;
    virtual Vec3<f32> getPosition() const         = 0;

    virtual void update(float deltaTime) = 0;

protected:
    Vec2<u32> m_viewportSize;

private:
    EventHandlerSentinel m_eventSentinel;
};

}  // namespace sl