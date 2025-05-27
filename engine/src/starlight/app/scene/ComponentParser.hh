#pragma once

#include <kstd/serialization/All.hh>

#include "starlight/app/scene/Entity.hh"

namespace sl {

struct ComponentParser {
    virtual void deserialize(Entity&, kstd::BinaryReader& br) const   = 0;
    virtual void serialize(const void*, kstd::BinaryWriter& bw) const = 0;
    virtual std::string getComponentName() const                      = 0;
};

template <typename T> class ComponentParserBase : public ComponentParser {
private:
    void serialize(const void* component, kstd::BinaryWriter& bw) const override {
        serialize(*static_cast<const T*>(component), bw);
    }

    virtual void serialize(const T& component, kstd::BinaryWriter& bw) const = 0;
};

}  // namespace sl
