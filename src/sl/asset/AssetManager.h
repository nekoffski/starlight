#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Asset.h"

namespace sl::asset {

class AssetManager {
    using NameToAssetMap = std::unordered_map<std::string, std::shared_ptr<Asset>>;
    using TypeToAssetMapMap = std::unordered_map<AssetType, NameToAssetMap>;
    using NamesVector = std::vector<std::string>;
    using TypeToNamesMap = std::unordered_map<AssetType, NamesVector>;

public:
    TypeToAssetMapMap& getAllAssets() {
        return m_resources;
    }

    NameToAssetMap& getAssetsByType(AssetType type) {
        if (m_resources.contains(type))
            return m_resources[type];
        return m_emptyAssetMap;
    }

    TypeToNamesMap& getAllNames() {
        return m_resourceNames;
    }

    NamesVector& getNamesByType(AssetType type) {
        if (m_resourceNames.contains(type))
            return m_resourceNames[type];
        return m_emptyNamesVector;
    }

    void addAsset(std::shared_ptr<Asset> resource) {
        auto type = resource->getType();
        m_resourceNames[type].push_back(resource->name);
        m_resources[type][resource->name] = resource;
    }

private:
    TypeToAssetMapMap m_resources;
    TypeToNamesMap m_resourceNames;

    inline static NamesVector m_emptyNamesVector;
    inline static NameToAssetMap m_emptyAssetMap;
};
}
