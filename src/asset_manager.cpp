#include "asset_manager.h"
#include <physfs.h>

void AssetManager::Init(const char *path) {
 PHYSFS_init(path);
}

void AssetManager::Cleanup() {
  PHYSFS_deinit();
}

bool AssetManager::Mount(const char* path, const char* target) {
  int err = PHYSFS_mount(path, target, 1);
  return (err != 0);
}

char* AssetManager::ReadBytes(const char* filename) {
  if (!PHYSFS_exists(filename)) {
    return nullptr;
  }
  PHYSFS_file* assetFile = PHYSFS_openRead(filename);
  PHYSFS_sint64 filesize = PHYSFS_fileLength(assetFile);
  PHYSFS_seek(assetFile, 0);
  char* buf;
  buf = new char[filesize+1];
  buf[filesize] = 0;
  PHYSFS_readBytes(assetFile, buf, filesize);
  PHYSFS_close(assetFile);
  return buf;
}

SDL_Surface* AssetManager::LoadSurface(const char* filename) {
  if (surface_cache_.count(filename) > 0) {
    return surface_cache_[filename];
  }

  char* buf;
  buf = ReadBytes(filename);
  SDL_RWops *rw = SDL_RWFromMem(buf, sizeof(buf));
  SDL_Surface* s = IMG_Load_RW(rw, 0);
  surface_cache_[filename] = s;
  return s;
}

const char* AssetManager::GetLastError() {
  return PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
}

int AssetManager::SurfaceCacheSize() {
  return surface_cache_.size();
}

