#pragma once

#include <memory>

namespace sl::asset::loaders {

template <typename T>
struct LoaderArgs {
};

template <typename T>
struct Loader {
};

template <typename T>
class IsSpecializedFor {
    using Yes = char;
    using No = int;

public:
    static constexpr bool value = (sizeof(test<T>(0)) == sizeof(Yes));

private:
    template <typename U>
    static Yes test(decltype(&U::load));

    template <typename U>
    static No test(...);
};
}
