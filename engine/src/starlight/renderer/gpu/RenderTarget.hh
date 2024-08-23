#pragma once

#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/core/math/Core.hh"

#include "fwd.hh"

namespace sl {

class RenderTarget {
public:
    enum class SyncMode { noSync, syncWithWindowSize };

    struct Properties {
        std::vector<Texture*> attachments;
        Vec2<u32> size;
    };

    explicit RenderTarget(u32 id, const Properties& props);
    virtual ~RenderTarget() = default;

    u32 getId() const;

    virtual void regenerate(const Properties& properties) = 0;

protected:
    u32 m_id;
    Properties m_props;
};

}  // namespace sl