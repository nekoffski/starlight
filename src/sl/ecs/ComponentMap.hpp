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
    std::shared_ptr<ComponentContainer<T>> get() {
        auto id = core::typeIndex<T>();
        if (not m_componentContainers.contains(id))
            m_componentContainers[id] = std::make_shared<ComponentContainer<T>>();
        return std::static_pointer_cast<ComponentContainer<T>>(m_componentContainers[id]);
    }

    void clear() {
        m_componentContainers.clear();
    }

    template <typename T>
    std::shared_ptr<ComponentContainer<T>> getByIndex(std::type_index id) {
        return std::static_pointer_cast<ComponentContainer<T>>(m_componentContainers[id]);
    }

    template <typename T>
    bool exists() {
        return m_componentContainers.contains(core::typeIndex<T>());
    }

private:
    std::unordered_map<std::type_index, std::shared_ptr<IComponentContainer>> m_componentContainers;
};
}
