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
  const char* f = "missing-file";
  ASSERT_EQ(NULL, a.ReadBytes(f));
}

TEST(AssetManagerTest, ReadFromFilesystem) {
  AssetManager a;
  a.Init(margv[0]);
  ASSERT_TRUE(a.Mount("test/golden", "/"));
  const char* f = "/example.txt";
  ASSERT_STREQ("example\n", a.ReadBytes(f));
}

TEST(AssetManagerTest, ReadFromArchive) {
  AssetManager a;
  a.Init(margv[0]);
  ASSERT_TRUE(a.Mount("test/golden.zip", "/"));
  const char* f = "/example.txt";
  ASSERT_STREQ("example\n", a.ReadBytes(f));
}

TEST(AssetManagerTest, SurfaceCache) {
  AssetManager a;
  a.Init(margv[0]);
  ASSERT_TRUE(a.Mount("test/golden.zip", "/"));
  const char* f = "/bar.png";
  SDL_Surface* s1 = a.LoadSurface(f);
  SDL_Surface* s2 = a.LoadSurface(f);
  ASSERT_EQ(1, a.SurfaceCacheSize());
  ASSERT_EQ(s1, s2);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  margv = argv;
  return RUN_ALL_TESTS();
}
