#include "asset_manager.h"
#include <physfs.h>
#include <iostream>

AssetManager::AssetManager() {
  PHYSFS_init(NULL);
}

AssetManager::~AssetManager() {
  PHYSFS_deinit();
}

bool AssetManager::Mount(const char* path, const char* target) {
  int err = PHYSFS_mount(path, target, 1);
  if (err == 0) {
    std::cerr << "AssetManager: " << path << " " << GetLastError() << std::endl;
  }
  return (err != 0);
}

SDL_Surface* AssetManager::LoadSurface(const char* filename) {
  if (!PHYSFS_exists(filename)) {
    std::cerr << "AssetManager: " << filename << " does not exist." << std::endl;
    return nullptr;
  }

  PHYSFS_file* assetFile = PHYSFS_openRead(filename);
  PHYSFS_sint64 filesize = PHYSFS_fileLength(assetFile);
  PHYSFS_seek(assetFile, 0);
  char* buf = new char[filesize+1];
  buf[filesize] = 0;
  int bytesRead = PHYSFS_readBytes(assetFile, buf, filesize);
  std::cout << "AssetManager: " << filename << " (" << bytesRead << " bytes)" << std::endl;
  PHYSFS_close(assetFile);
  SDL_RWops *rw = SDL_RWFromConstMem(buf, filesize);
  if (!rw) {
    return nullptr;
  }
  SDL_Surface* s = IMG_Load_RW(rw, 0);
  if (!s) {
    return nullptr;
  }
  return s;
}

void AssetManager::FreeSurface(SDL_Surface* s) {
  SDL_FreeSurface(s);
}

SDL_Texture* AssetManager::LoadTexture(const char* filename, SDL_Renderer* r) {
  if (texture_cache_.count(filename) > 0) {
    return texture_cache_.at(filename);
  }
  SDL_Surface* s = LoadSurface(filename);
  if (s == nullptr) {
    return nullptr;
  }
  SDL_Texture* t = SDL_CreateTextureFromSurface(r, s);
  SDL_FreeSurface(s);
  texture_cache_[filename] = t;
  return t;
}

const char* AssetManager::GetLastError() {
  return PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
}

