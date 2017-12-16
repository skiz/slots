#include "asset_manager.h"
#include <physfs.h>
#include <iostream>

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

/*
char* AssetManager::ReadBytes(const char* filename) {
  if (!PHYSFS_exists(filename)) {
    std::cerr << filename << " does not exist." << std::endl;
    return nullptr;
  }
  PHYSFS_file* assetFile = PHYSFS_openRead(filename);
  PHYSFS_sint64 filesize = PHYSFS_fileLength(assetFile);
  PHYSFS_seek(assetFile, 0);
  char* buf = new char[filesize+1];
  buf[filesize] = 0;
  int bytesRead = PHYSFS_readBytes(assetFile, buf, filesize);
  std::cout << bytesRead << " " << filename << std::endl;
  PHYSFS_close(assetFile);
  return buf;
}
*/

SDL_Surface* AssetManager::LoadSurface(const char* filename) {
  if (!PHYSFS_exists(filename)) {
    std::cerr << filename << " does not exist." << std::endl;
    return nullptr;
  }

  PHYSFS_file* assetFile = PHYSFS_openRead(filename);
  PHYSFS_sint64 filesize = PHYSFS_fileLength(assetFile);
  PHYSFS_seek(assetFile, 0);
  char* buf = new char[filesize+1];
  buf[filesize] = 0;
  int bytesRead = PHYSFS_readBytes(assetFile, buf, filesize);
  std::cout << bytesRead << " " << filename << std::endl;
  PHYSFS_close(assetFile);
  SDL_RWops *rw = SDL_RWFromConstMem(buf, filesize);
  SDL_Surface* s = IMG_Load_RW(rw, 0);
  return s;
}

const char* AssetManager::GetLastError() {
  return PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
}

