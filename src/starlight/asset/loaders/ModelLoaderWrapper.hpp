#pragma once

#include <starlight/asset/loaders/AssetLoader.h>
#include <starlight/core/path/PathManager.hpp>

#include <starlight/platform/model/ModelLoader.h>

#include <starlight/core/log/Logger.h>

namespace starl::asset::loaders {

using geometry::Model;
using platform::model::ModelLoader;

template <>
struct AssetLoaderArgs<Model> {
    std::string path;
};

class ModelLoaderWrapper : public AssetLoader<Model> {
public:
    std::shared_ptr<Model> load(AssetLoaderArgs<Model> args) {
        static auto modelLoader{ ModelLoader::create() };
        LOG(DEBUG) << "loading model " << args.path;
        return modelLoader->loadModel(core::path::PathManager::createGlobalPath<Model>(args.path));
    }
};
}
