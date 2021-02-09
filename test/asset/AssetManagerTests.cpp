#include "sl/asset/AssetManager.h"

#include <gtest/gtest.h>

using namespace testing;
using namespace sl::asset;

namespace {

class AssetManagerTests : public Test {
public:
	AssetManager m_assetManager;
};

TEST_F(AssetManagerTests, givenEmptyAssetManager_whenGettingAllAssets_shouldReturnEmptyContainers) {
	EXPECT_EQ(m_assetManager.getAllAssets().size(), 0);	
}

TEST_F(AssetManagerTests, givenEmptyAssetManager_whenGettingAssetsByType_shouldReturnEmptyContainers) {
	EXPECT_EQ(m_assetManager.getAssetsByType(AssetType::model).size(), 0);	
}

TEST_F(AssetManagerTests, givenEmptyAssetManager_whenGettingAllAssetsNames_shouldReturnEmptyContainers) {
	EXPECT_EQ(m_assetManager.getAssetsNames().size(), 0);	
}


}
