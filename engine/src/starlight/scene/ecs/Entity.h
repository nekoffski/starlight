#pragma once

#include <fmt/core.h>

#include "starlight/core/Core.h"

namespace sl {

class Entity {
public:
    explicit Entity(u64 id, const std::string& name) : m_id(id), m_name(name) {}
    explicit Entity(u64 id) : Entity(id, fmt::format("Entity_{}", id)) {}

    const u64 getId() const { return m_id; }
    const std::string& getName() const { return m_name; }

private:
    u64 m_id;
    std::string m_name;
};

}  // namespace sl
