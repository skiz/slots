#include "asset_manager.h"
#include <gtest/gtest.h>

class AssetManagerTest : public ::testing::Test {
  protected:
    AssetManager* a_;
    SDL_Renderer* renderer_;
    SDL_Window* window_;

    virtual void SetUp() {
      window_ = SDL_CreateWindow("TEST",
          SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          0, 0, 0);
      renderer_ = SDL_CreateRenderer(window_, -1, 0);
      a_ = &AssetManager::GetInstance();
      a_->Mount("test/golden", "/");
    }
};

TEST_F(AssetManagerTest, MissingMount) {
  ASSERT_FALSE(a_->Mount("missing", "/"));
}

TEST_F(AssetManagerTest, Mount_ValidDirectory) {
  ASSERT_TRUE(a_->Mount("test/golden", "/"));
}

TEST_F(AssetManagerTest, Mount_ValidPackage) {
  ASSERT_TRUE(a_->Mount("test/golden.zip", "/"));
}

TEST_F(AssetManagerTest, LoadSurface_MissingFile) {
  ASSERT_EQ(nullptr, a_->LoadSurface("missing.png"));
}

TEST_F(AssetManagerTest, LoadSurface_InvalidImage) {
  ASSERT_EQ(nullptr, a_->LoadSurface("example.txt"));
}

TEST_F(AssetManagerTest, LoadSurface_ValidFile) {
  ASSERT_NE(nullptr, a_->LoadSurface("bar.png"));
}

TEST_F(AssetManagerTest, LoadTexture_InvalidFile) {
  SDL_Texture* t = a_->LoadTexture("example.txt", renderer_);
  ASSERT_EQ(nullptr, t);
}

TEST_F(AssetManagerTest, LoadTexture_ValidFile) {
  SDL_Texture* t = a_->LoadTexture("bar.png", renderer_);
  ASSERT_NE(nullptr, t);
}

