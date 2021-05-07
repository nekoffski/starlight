#pragma once

#include <memory>

#include "sl/core/Json.h"
#include "sl/core/Logger.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gui/Utils.hpp"
#include "sl/gui/fonts/FontAwesome.h"
#include "sl/math/Utils.hpp"
#include "sl/math/Vector.hpp"

namespace sl::scene::components {

class TransformComponent : public ecs::Component {
    struct Memento {
        math::Vec3 position;
        math::Vec3 rotation;
        math::Vec3 scale;
    };

public:
    explicit TransformComponent(math::Vec3 position = math::Vec3 { 0.0f },
        math::Vec3 rotation = math::Vec3 { 0.0f }, math::Vec3 scale = math::Vec3 { 1.0f })
        : position(position)
        , rotation(rotation)
        , scale(scale) {
        recalculateTransformation();
    }

    void recalculateTransformation() {
        transformation = math::translate(position) * math::scale(scale) *
            math::createRotationMatrix(math::toRadians(rotation));
    }

    void save() {
        m_memento = Memento {
            position, rotation, scale
        };
    }

    void restore() {
        SL_INFO("Trying to restore transform");
        if (m_memento.has_value()) {
            SL_INFO("Restoring transform component");
            position = m_memento->position;
            rotation = m_memento->rotation;
            scale = m_memento->scale;

            recalculateTransformation();

            m_memento.reset();
        }
    }

    math::Vec3 position;
    math::Vec3 rotation;
    math::Vec3 scale;
    math::Mat4 transformation;

private:
    std::optional<Memento> m_memento;
};
}
