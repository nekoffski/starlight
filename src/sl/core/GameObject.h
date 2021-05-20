#pragma once

#include "sl/core/Logger.h"

#include "Countable.hpp"

namespace sl::core {

struct GameObject : Countable<GameObject> {
    GameObject() {
        SL_INFO("GameObject - taking id: {}", id);
    }

    ~GameObject() {
        SL_INFO("GameObject - freeing id: {}", id);
    }
};

}