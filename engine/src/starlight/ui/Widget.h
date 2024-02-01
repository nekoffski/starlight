#pragma once

#include <functional>

namespace sl::ui {

struct Widget {
    using Callback = std::function<void()>;

    virtual void render() = 0;
};

}  // namespace sl::ui
