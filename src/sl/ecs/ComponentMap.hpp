#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "sl/core/misc/misc.hpp"

namespace sl::ecs {

template <typename IContainer, template <typename> typename Container>
class ComponentMap {
public:
    template <typename T>
    std::shared_ptr<Container<T>> get() {
        auto id = core::misc::typeIndex<T>();
        if (not m_componentContainers.contains(id))
            m_componentContainers[id] = Container<T>::create();
        return std::static_pointer_cast<Container<T>>(m_componentContainers[id]);
    }

    template <typename T>
    std::shared_ptr<Container<T>> getByIndex(std::type_index id) {
        return std::static_pointer_cast<Container<T>>(m_componentContainers[id]);
    }

    template <typename T>
    bool exists() {
        return m_componentContainers.contains(core::misc::typeIndex<T>());
    }

private:
	std::unordered_map<std::type_index, std::shared_ptr<IContainer>> m_componentContainers;
};
}
