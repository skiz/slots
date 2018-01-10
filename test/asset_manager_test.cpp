#include "asset_manager.h"
#include <gtest/gtest.h>

class AssetManagerTest : public ::testing::Test {
  protected:
    AssetManager* a_;
    SDL_Renderer* renderer_;
    SDL_Window* window_;

    static const int AUDIO_FREQUENCY = 44100;
    static const int AUDIO_CHANNELS = 2;
    static const int AUDIO_SAMPLE_SIZE = 2048;

    virtual void SetUp() {
      SDL_InitSubSystem(SDL_INIT_AUDIO);
      Mix_OpenAudio(AUDIO_FREQUENCY, MIX_DEFAULT_FORMAT,
                    AUDIO_CHANNELS, AUDIO_SAMPLE_SIZE);
      
      window_ = SDL_CreateWindow("TEST",
          SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          0, 0, 0);
      renderer_ = SDL_CreateRenderer(window_, -1, 0);

      TTF_Init();

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
  SDL_Surface* s = a_->LoadSurface("bar.png");
  ASSERT_NE(nullptr, s);
  a_->FreeSurface(s);
}

TEST_F(AssetManagerTest, LoadTexture_InvalidFile) {
  SDL_Texture* t = a_->LoadTexture("example.txt", renderer_);
  ASSERT_EQ(nullptr, t);
}

TEST_F(AssetManagerTest, LoadTexture_ValidFile) {
  SDL_Texture* t = a_->LoadTexture("bar.png", renderer_);
  ASSERT_NE(nullptr, t);
}

TEST_F(AssetManagerTest, LoadSurface_NotCached) {
  SDL_Surface* s1 = a_->LoadSurface("bar.png");
  SDL_Surface* s2 = a_->LoadSurface("bar.png");
  ASSERT_NE(s1, s2) << "Expected same surface pointer";
  a_->FreeSurface(s1);
  a_->FreeSurface(s2);
}

TEST_F(AssetManagerTest, LoadTexture_Cached) {
  SDL_Texture* t1 = a_->LoadTexture("bar.png", renderer_);
  SDL_Texture* t2 = a_->LoadTexture("bar.png", renderer_);
  ASSERT_EQ(t1, t2) << "Expected same texture pointer";
}

TEST_F(AssetManagerTest, LoadSound_Package) {
  ASSERT_NE(nullptr, a_->LoadSound("reels.wav"));
}

TEST_F(AssetManagerTest, LoadMusic_Package) {
  ASSERT_NE(nullptr, a_->LoadSound("reels.wav"));
}

TEST_F(AssetManagerTest, LoadFont_Cache) {
  TTF_Font* f1 = a_->LoadFont("sans.ttf", 40);
  TTF_Font* f2 = a_->LoadFont("sans.ttf", 40);
  ASSERT_EQ(f1, f2);
}

TEST_F(AssetManagerTest, LoadFont_NoCache) {
  TTF_Font* f1 = a_->LoadFont("sans.ttf", 190);
  TTF_Font* f2 = a_->LoadFont("digital.ttf", 1200);
  ASSERT_NE(f1, f2) << "Font is not scalable, so pointer is the same";
}


