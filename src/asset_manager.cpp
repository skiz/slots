#include "asset_manager.h"
#include <physfs.h>

void AssetManager::Init(const char *path) {
 PHYSFS_init(path);
}

void AssetManager::Cleanup() {
  PHYSFS_deinit();
}

bool AssetManager::AddSearchPath(const char* path) {
  int err = PHYSFS_mount(path, "/", 1);
  return (err == 0);
}

char* AssetManager::ReadBytes(const char* filename) {
  if (!PHYSFS_exists(filename)) {
    return nullptr;
  }
  PHYSFS_file* assetFile = PHYSFS_openRead(filename);
  PHYSFS_sint64 filesize = PHYSFS_fileLength(assetFile);
  char* buf;
  buf = new char[filesize];
  PHYSFS_readBytes(assetFile, buf, filesize);
  PHYSFS_close(assetFile);
  return buf;
}

