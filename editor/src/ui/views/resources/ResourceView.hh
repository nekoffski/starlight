#pragma once

#include "ui/Resource.hh"

namespace sle {

struct ResourceView {
    virtual void render(Resource& resource) = 0;
};

}  // namespace sle
