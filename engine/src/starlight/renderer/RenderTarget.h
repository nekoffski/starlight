#pragma once

#include <vector>

#include "starlight/core/Core.h"

#include "fwd.h"

namespace sl {

class RenderTarget {
public:
    enum class SyncMode { noSync, syncWithWindowSize };

    struct Properties {
        std::vector<Texture*> attachments;
        u32 width;
        u32 height;
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