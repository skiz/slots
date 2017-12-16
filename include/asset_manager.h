#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <unordered_map>
#include "SDL_Image.h"

/**
 * Handles loading of asset files in both the filesystem and
 * archives.  Things such as music, images, etc are handled here.
 */
class AssetManager {
  public:
    void Init(const char* path);
    void Cleanup();
    bool Mount(const char* path, const char* target);
    //char* ReadBytes(const char* filename);
    SDL_Surface* LoadSurface(const char* filename);
    const char* GetLastError();
};

#endif
