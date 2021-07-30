#pragma once

#include <memory>

#include "fwd.h"

namespace sl::geom {

struct ModelLoader {
    virtual ~ModelLoader() = default;

    virtual std::shared_ptr<geom::Model> load(const std::string&) = 0;
};

}
