#include "asset_manager.h"
#include <gtest/gtest.h>

char** margv;

TEST(AssetManagerTest, MissingMount) {
  AssetManager *a = &AssetManager::GetInstance();
  ASSERT_FALSE(a->Mount("missing", "/"));
}
