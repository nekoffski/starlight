#pragma once

#include "starlight/core/Json.hh"
#include "starlight/app/scene/Entity.hh"

namespace sl {

struct ComponentParser {
    virtual void deserialize(Entity&, const nlohmann::json&) const = 0;
    virtual nlohmann::json serialize(const void*) const            = 0;
    virtual std::string getComponentName() const                   = 0;
};

template <typename T> class ComponentParserBase : public ComponentParser {
private:
    nlohmann::json serialize(const void* component) const override {
        return serialize(*static_cast<const T*>(component));
    }

    virtual nlohmann::json serialize(const T& component) const = 0;
};

}  // namespace sl
