#pragma once

#include <vector>

#include "starlight/core/Core.h"
#include "starlight/core/math/Core.h"

#include "fwd.h"

namespace sl {

class RenderTarget {
public:
    enum class SyncMode { noSync, syncWithWindowSize };

    struct Properties {
        std::vector<Texture*> attachments;
        Vec2u32 size;
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