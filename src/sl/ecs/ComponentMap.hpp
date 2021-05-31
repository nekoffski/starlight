#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "sl/core/Misc.hpp"
#include "sl/ecs/ComponentContainer.hpp"

namespace sl::ecs {

class ComponentMap {
public:
    template <typename T>
    ComponentContainer<T>& get() {
        auto id = core::typeIndex<T>();
        if (not m_componentContainers.contains(id))
            m_componentContainers[id] = std::make_unique<ComponentContainer<T>>();
        return static_cast<ComponentContainer<T>&>(*m_componentContainers[id]);
    }

    void clear();
    IComponentContainer& getByIndex(std::type_index id);

    template <typename T>
    bool exists() {
        return m_componentContainers.contains(core::typeIndex<T>());
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<IComponentContainer>> m_componentContainers;
};
}
