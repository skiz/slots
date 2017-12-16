#include "asset_manager.h"
#include <gtest/gtest.h>

TEST(AssetManagerTest, ReadMissingFile) {
  AssetManager a;
  a.Init("foo");
  const char *fn = "missing-file";
  ASSERT_EQ(NULL, a.ReadBytes(fn));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
