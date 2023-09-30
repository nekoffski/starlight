#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <array>

#include "starlight/core/Id.hpp"
#include "starlight/core/Core.h"

#include "Texture.h"
#include "ShaderScope.h"
#include "ShaderUniform.h"
#include "ShaderAttribute.h"

namespace sl {

class Shader {
public:
    using Stage = u16;

    static constexpr Stage stageVertex   = 0x1;
    static constexpr Stage stageGeometry = 0x2;
    static constexpr Stage stagePixel    = 0x4;
    static constexpr Stage stageComputer = 0x8;

    enum class State : u8 { notCreated, uninitialized, initialized };

    virtual void use()         = 0;
    virtual void bindGlobals() = 0;

    u16 getUniformIndex(const std::string& uniformName) {}

    Id32 getId() const { return m_id; }

protected:
    Id32 m_id;
    std::string m_name;

    bool m_useInstances;
    bool m_useLocals;

    u64 m_requiredUboAlignment;

    u64 m_globalUboSize;
    u64 m_globalUboStride;
    u64 m_globalUboOffset;

    u64 m_uboSize;
    u64 m_uboStride;

    u64 m_pushConstantSize;
    u64 m_pushConstantStride;

    std::vector<Texture*> m_globalTextures;
    u8 m_instanceTextureCount;

    ShaderScope m_boundScope;

    u32 m_boundInstanceId;
    u32 m_boundUboOffset;

    // std::unordered_map m_uniformLut;

    std::vector<ShaderUniform> m_uniforms;
    std::vector<ShaderAttribute> m_attributes;

    State m_state;

    u8 m_pushConstantRangeCount;
    std::array<Range, 32> m_pushConstantRanges;

    u16 m_attributeStride;
};

}  // namespace sl
