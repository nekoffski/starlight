#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "nova/core/FileSystem.h"
#include "nova/core/Memory.hpp"
#include "nova/core/Core.hpp"

#include "Geometry.h"
#include "ResourceProxy.h"

#include "fwd.h"

namespace nova::gfx {

class GeometryManager : public kc::core::Singleton<GeometryManager> {
   public:
    explicit GeometryManager() {}

    Geometry* acquire(uint32_t id) {}

   private:
};

}  // namespace nova::gfx