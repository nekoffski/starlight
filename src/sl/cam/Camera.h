#pragma once

#include <kc/core/Log.h>
#include <kc/event/EventListener.h>

#include <cstdint>

#include "sl/gfx/ViewFrustum.h"
#include "sl/math/Common.h"
#include "sl/math/Matrix.hpp"
#include "sl/math/Vector.hpp"

namespace sl::cam {

class Camera : kc::event::EventListener {
   public:
    explicit Camera(
        const gfx::ViewFrustum &viewFrustum, const math::Vec3 &position = math::zerosVector,
        const math::Vec3 &up = math::worldUp, const math::Vec3 &front = math::worldFront,
        const math::Vec3 &right = math::worldRight
    );

    virtual ~Camera();

    void handleEvents(const kc::event::EventProvider &eventProvider) override;

    virtual void update(float) = 0;
    virtual void onGui();

    const math::Vec3 &getPosition() const;
    const math::Vec3 &getUp() const;
    const math::Vec3 &getFront() const;
    const math::Vec3 &getRight() const;
    const math::Mat4 &getProjectionMatrix() const;

    void calculateProjectionMatrix();

    virtual const math::Mat4 getViewMatrix() const;

    gfx::ViewFrustum viewFrustum;

   protected:
    math::Vec3 m_position;
    math::Vec3 m_up;
    math::Vec3 m_front;
    math::Vec3 m_right;

    math::Mat4 m_projectionMatrix;
};
}  // namespace sl::cam
