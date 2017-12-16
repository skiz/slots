#include "asset_manager.h"
#include <gtest/gtest.h>

char** margv;

TEST(AssetManagerTest, MissingMount) {
  AssetManager a;
  a.Init(margv[0]);
  ASSERT_FALSE(a.Mount("missing", "/"));
}

TEST(AssetManagerTest, ReadMissingFile) {
  AssetManager a;
  a.Init(margv[0]);
  const char *f = "missing-file";
  ASSERT_EQ(NULL, a.ReadBytes(f));
}

TEST(AssetManagerTest, ReadFromFilesystem) {
  AssetManager a;
  a.Init(margv[0]);
  ASSERT_TRUE(a.Mount("test/golden", "/"));
  const char *f = "/example.txt";
  ASSERT_STREQ("example\n", a.ReadBytes(f));
}

TEST(AssetManagerTest, ReadFromArchive) {
  AssetManager a;
  a.Init(margv[0]);
  ASSERT_TRUE(a.Mount("test/golden.zip", "/"));
  const char *f = "/example.txt";
  ASSERT_STREQ("example\n", a.ReadBytes(f));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  margv = argv;
  return RUN_ALL_TESTS();
}
