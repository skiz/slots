#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <unordered_map>

/**
 * Handles loading and caching of assets from the filesystem.
 *
 * Surfaces are not cached, and must be deleted manually.
 * Textures are cached, and AssetManager retains ownership.
 *
 */
class AssetManager {
  public:
    static AssetManager& GetInstance() {
      static AssetManager instance;
      return instance;
    }
    bool Mount(std::string src, std::string dest);
    SDL_Surface* LoadSurface(std::string filename);
    void FreeSurface(SDL_Surface* s);
    SDL_Texture* LoadTexture(std::string filename, SDL_Renderer* r);
    Mix_Chunk* LoadSound(std::string filename);
    Mix_Music* LoadMusic(std::string filename);
    TTF_Font* LoadFont(std::string filename, int size);
    AssetManager(AssetManager const&) = delete;
    void operator=(AssetManager const&) = delete;

  private:
    AssetManager();
    ~AssetManager();
    static AssetManager* instance;
    SDL_RWops* ReadFileContents(std::string filename);
    std::string GetLastError();
    std::unordered_map<std::string, SDL_Texture*> texture_cache_;
    std::unordered_map<std::string, Mix_Chunk*> sound_cache_;
    std::unordered_map<std::string, Mix_Music*> music_cache_;
    std::unordered_map<std::string, TTF_Font*> font_cache_;
};

#endif
