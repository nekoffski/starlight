#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Asset.h"
#include "sl/core/Logger.h"

namespace sl::asset {

class AssetManager {
    using NameToAssetMap = std::unordered_map<std::string, std::shared_ptr<Asset>>;
	using IdToAssetMap = std::unordered_map<unsigned long, std::shared_ptr<Asset>>;
	using TypeToAssetMapMap = std::unordered_map<AssetType, NameToAssetMap>;
    using NamesVector = std::vector<std::string>;
    using TypeToNamesMap = std::unordered_map<AssetType, NamesVector>;

public:
    TypeToAssetMapMap& getAllAssets();
    NameToAssetMap& getAssetsByType(AssetType type);
    TypeToNamesMap& getAssetsNames();
    NamesVector& getNamesByType(AssetType type);

    // TODO: optimize
    std::shared_ptr<Asset> getAssetById(unsigned long id) {
		return m_idToAsset[id];
	}

    void addAsset(std::shared_ptr<Asset> resource);

private:
    TypeToAssetMapMap m_assets;
    TypeToNamesMap m_assetsNames;
	IdToAssetMap m_idToAsset;

    inline static NamesVector m_emptyNamesVector;
    inline static NameToAssetMap m_emptyAssetMap;
};
}
