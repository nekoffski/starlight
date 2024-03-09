#pragma once

#include <vector>
#include <string>
#include <concepts>

#include "starlight/core/Core.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/core/math/Vertex.h"
#include "starlight/core/utils/Log.h"

namespace sl {

template <typename T, typename... Args>
concept ConstructibleWithId = requires(Args&&... args) {
    { T(u64{}, std::forward<Args>(args)...) };
};

template <typename T> class ResourcePool {
public:
    explicit ResourcePool(const std::string& resourceName, u64 size) :
        m_resourceName(resourceName), m_size(size), m_buffer(size) {
        LOG_INFO("Creating resource pool for '{}' of size {}", resourceName, size);
    }

    template <typename... Args>
    T* create(Args&&... args) requires ConstructibleWithId<T, Args...> {
        LOG_DEBUG("Looking for free {} slot", m_resourceName);
        for (int i = 0; i < m_size; ++i) {
            if (not m_buffer[i]) {
                LOG_DEBUG("Slot {} found, will create {}", i, m_resourceName);
                m_buffer[i].emplace(i, std::forward<Args>(args)...);
                return m_buffer[i].get();
            }
        }
        LOG_WARN(
          "Couldn't find slot for a new {}, consider changing configuration to allow more",
          m_resourceName
        );
        return nullptr;
    }

    void clear() {
        m_buffer.clear();
        m_buffer.resize(m_size);
    }

    void destroy(u64 id) { m_buffer[id].clear(); }

    T* get(u64 id) { return m_buffer.at(id).get(); }
    u64 getSize() const { return m_size; }

    const std::string getResourceName() const { return m_resourceName; }

private:
    std::string m_resourceName;
    u64 m_size;
    std::vector<LocalPtr<T>> m_buffer;
};

}  // namespace sl