#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "SDL_image.h"
#include <iostream>

/**
 * Handles loading and caching of assets from the filesystem
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
    ~AssetManager();
    bool Mount(const char* src, const char* dest);
    SDL_Surface* LoadSurface(const char* filename);
    SDL_Texture* LoadTexture(const char* filename, SDL_Renderer* r);
    
    AssetManager(AssetManager const&) = delete;
    void operator=(AssetManager const&) = delete;

  private:
    const char* GetLastError();
    AssetManager();
    static AssetManager* instance;
};

#endif
