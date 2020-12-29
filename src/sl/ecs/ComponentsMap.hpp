#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "sl/core/misc/misc.hpp"

namespace sl::ecs {

template <typename IContainer, template <typename> typename Container, template <typename, typename> typename Map = std::unordered_map>
class ComponentsMap {
public:
    template <typename Component>
    std::shared_ptr<Container<Component>> get() {
        auto id = core::misc::typeIndex<Component>();
        if (m_componentsContainers.count(id) < 1)
            m_componentsContainers[id] = Container<Component>::create();
        return std::static_pointer_cast<Container<Component>>(m_componentsContainers[id]);
    }

    template <typename Component>
    std::shared_ptr<Container<Component>> getByIndex(std::type_index id) {
        return std::static_pointer_cast<Container<Component>>(m_componentsContainers[id]);
    }

    template <typename Component>
    bool exists() {
        auto id = core::misc::typeIndex<Component>();
        return m_componentsContainers.count(id) > 0;
    }

private:
    Map<std::type_index, std::shared_ptr<IContainer>> m_componentsContainers;
};
}
