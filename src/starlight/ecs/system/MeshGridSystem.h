#pragma once

#include "System.h"

#include "starlight/core/log/Logger.h"
#include "starlight/ecs/component/MeshGridComponent.h"
#include "starlight/math/Utils.hpp"
#include "starlight/rendering/data/ModelRenderData.h"

namespace sl::ecs::system {

class MeshGridSystem : public System {
    void processComponent(std::shared_ptr<component::Component>& component, std::shared_ptr<entity::Entity>& entity) override {
        processComponentImpl(std::dynamic_pointer_cast<component::MeshGridComponent>(component));
    }

private:
    void processComponentImpl(std::shared_ptr<component::MeshGridComponent> component) {
        auto [x, z, width, length] = component->getGridParams();
        auto gridRenderData = component->getGridRenderData();

        // TODO: optimize
        gridRenderData->modelMatrices.clear();
        auto scale = math::scale({ width, 0.0f, length });
        int halfx = x / 2;
        int halfz = z / 2;
        for (int i = 0; i < x; ++i)
            for (int j = 0; j < z; ++j)
                gridRenderData->modelMatrices.push_back(
                    math::translate({ (i - halfx) * width, 0.0f, (j - halfz) * length }) * scale);
    }
};
}
