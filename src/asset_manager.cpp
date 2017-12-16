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

const char* AssetManager::GetLastError() {
  return PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
}


