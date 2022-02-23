#pragma once

namespace sl::core {

struct Asset {
    virtual int getId() = 0;
};

}  // namespace sl::core