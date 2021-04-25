#include "AssetManager.h"

// namespace sl::asset {

// AssetManager::TypeToAssetMapMap& AssetManager::getAllAssets() {
//     return m_assets;
// }

// AssetManager::NameToAssetMap& AssetManager::getAssetsByType(AssetType type) {
//     if (m_assets.contains(type))
//         return m_assets[type];
//     return m_emptyAssetMap;
// }

// AssetManager::TypeToNamesMap& AssetManager::getAssetsNames() {
//     return m_assetsNames;
// }

// AssetManager::NamesVector& AssetManager::getNamesByType(AssetType type) {
//     if (m_assetsNames.contains(type))
//         return m_assetsNames[type];
//     return m_emptyNamesVector;
// }

// void AssetManager::addAsset(std::shared_ptr<Asset> asset) {
//     auto type = asset->getType();
//     m_assetsNames[type].push_back(asset->name);
//     m_assets[type][asset->name] = asset;
//     m_idToAsset[asset->getId()] = asset;
// }
// }
