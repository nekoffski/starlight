#pragma once

#include <fmt/core.h>

#include "starlight/core/Core.h"

#include "ComponentContainerMap.h"

namespace sl {

class Entity {
public:
    template <typename T> bool hasComponent() {
        return m_componentContainerMap.hasComponent<T>(m_id);
    }

    template <typename T, typename... Args> T* addComponent(Args&&... args) {
        return m_componentContainerMap.addComponent<T>(
          m_id, std::forward<Args>(args)...
        );
    }

    template <typename T> T* getComponent() {
        return m_componentContainerMap.hasComponent<T>(m_id)
                 ? m_componentContainerMap.getComponent<T>(m_id)
                 : nullptr;
    }

    explicit Entity(
      u64 id, const std::string& name, ComponentContainerMap& componentContainerMap
    );
    explicit Entity(u64 id, ComponentContainerMap& componentContainerMap);

    const u64 getId() const;
    const std::string& getName() const;

private:
    u64 m_id;
    std::string m_name;
    ComponentContainerMap& m_componentContainerMap;
};

}  // namespace sl
