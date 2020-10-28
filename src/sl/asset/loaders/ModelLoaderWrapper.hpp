#pragma once

#include "sl/asset/loaders/AssetLoader.h"

#include "sl/core/log/Logger.h"
#include "sl/core/path/PathManager.hpp"
#include "sl/platform/model/ModelLoader.h"
#include "sl/geometry/Model.h"

namespace sl::asset::loaders {

using geometry::Model;
using platform::model::ModelLoader;

template <>
struct AssetLoaderArgs<Model> {
    std::string path;
};

template <>
class AssetLoader<Model> {
public:
    static std::shared_ptr<Model> load(AssetLoaderArgs<Model> args) {

        SL_DEBUG("loading model: {}", args.path);
        return m_modelLoader->loadModel(core::path::PathManager::createGlobalPath<Model>(args.path));
    }

private:
    inline static auto m_modelLoader{ ModelLoader::create() };
};
}
