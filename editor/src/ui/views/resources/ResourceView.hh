#pragma once

#include <starlight/core/Core.hh>

namespace sle {

enum class ResourceType : sl::u16 {
    unknown,
    directory,
    shader,
    texture,
    material,
    model,
    wavefrontObject,
    wavefrontMaterial
};

}
