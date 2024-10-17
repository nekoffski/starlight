#pragma once

#include "starlight/core/utils/Id.hh"

namespace sl {

class Entity : public Identificable<Entity> {
public:
    explicit Entity();
    explicit Entity(const std::string& name);

private:
    std::string m_name;
};

}  // namespace sl
