#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "SDL_image.h"
#include <iostream>
#include <unordered_map>

/**
 * Handles loading and caching of assets from the filesystem.
 *
 * Surfaces are not cached, and must be deleted manually.
 * Textures are cached, and AssetManager retains ownership.
 *
 * DONE: surfaces, textures
 * TODO: music, sound, fonts
 */
class AssetManager {
  public:
    static AssetManager& GetInstance() {
      static AssetManager instance;
      return instance;
    };
    bool Mount(const char* src, const char* dest);
    SDL_Surface* LoadSurface(const char* filename);
    void FreeSurface(SDL_Surface* s);
    SDL_Texture* LoadTexture(const char* filename, SDL_Renderer* r);
    AssetManager(AssetManager const&) = delete;
    void operator=(AssetManager const&) = delete;

  private:
    AssetManager();
    ~AssetManager();
    static AssetManager* instance;
    const char* GetLastError();
    std::unordered_map<const char*, SDL_Texture*> texture_cache_;
};

#endif
