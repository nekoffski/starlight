#pragma once

#include <future>
#include <variant>

namespace sl {

struct RendererNoop {};

struct RendererFlush {
    std::promise<void> completion;
};

using RendererCommand = std::variant<RendererNoop, RendererFlush>;

}  // namespace sl
