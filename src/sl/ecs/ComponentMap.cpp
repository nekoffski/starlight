#include "ComponentMap.hpp"

namespace sl::ecs {

void ComponentMap::clear() { m_componentContainers.clear(); }

IComponentContainer& ComponentMap::getByIndex(std::type_index id) {
    return *m_componentContainers[id];
}

}  // namespace sl::ecs
