#pragma once

#include <memory>

namespace sl::asset::loaders {

template <typename T>
struct AssetLoaderArgs {
};

template <typename T>
class AssetLoader {
public:
    virtual std::shared_ptr<T> load(AssetLoaderArgs<T>) = 0;
};
}