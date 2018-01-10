#include "asset_manager.h"
#include <physfs.h>
#include <iostream>
#include <sstream>
#include <SDL_mixer.h>

AssetManager::AssetManager() {
  PHYSFS_init(NULL);
}

AssetManager::~AssetManager() {
  PHYSFS_deinit();
}

bool AssetManager::Mount(std::string path, std::string target) {
  int err = PHYSFS_mount(path.c_str(), target.c_str(), 1);
  if (err == 0) {
    std::cerr << "AssetManager: " << path << " " << GetLastError() << std::endl;
  }
  return (err != 0);
}

SDL_Surface* AssetManager::LoadSurface(std::string filename) {
   SDL_RWops* rw = ReadFileContents(filename);
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

SDL_Texture* AssetManager::LoadTexture(std::string filename, SDL_Renderer* r) {
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

Mix_Chunk* AssetManager::LoadSound(std::string filename) {
  if (sound_cache_.count(filename) > 0) {
    return sound_cache_.at(filename);
  }
  Mix_Chunk* snd = Mix_LoadWAV_RW(ReadFileContents(filename), 1);
  if (!snd) {
    std::cerr << "Mix_LoadWav: " << Mix_GetError() << std::endl;
    return nullptr;
  }
  sound_cache_[filename] = snd;
  return snd;
}

Mix_Music* AssetManager::LoadMusic(std::string filename) {
  if (music_cache_.count(filename) > 0) {
    return music_cache_.at(filename);
  }
  Mix_Music* m = Mix_LoadMUS_RW(ReadFileContents(filename), 1);
  if (!m) {
    std::cerr << "Mix_LoadMUS: " << Mix_GetError() << std::endl;
    return nullptr;
  }
  music_cache_[filename] = m;
  return m;
}

TTF_Font* AssetManager::LoadFont(std::string filename, int size) {
  std::string index = filename + std::to_string(size);
  if (font_cache_.count(index) > 0) {
    return font_cache_.at(index);
  }
  TTF_Font* f = TTF_OpenFontRW(ReadFileContents(filename), 1, size);
  if (!f) {
    std::cerr << "TTF_OpenFontRW: " << TTF_GetError() << std::endl;
    return nullptr;
  }
  font_cache_[index] = f;
  return f;
}

SDL_RWops* AssetManager::ReadFileContents(std::string filename) {
 if (!PHYSFS_exists(filename.c_str())) {
    std::cerr << "AssetManager: " << filename << " does not exist." << std::endl;
    return nullptr;
  }

  PHYSFS_file* assetFile = PHYSFS_openRead(filename.c_str());
  PHYSFS_sint64 filesize = PHYSFS_fileLength(assetFile);
  PHYSFS_seek(assetFile, 0);
  char* buf = new char[filesize+1];
  buf[filesize] = 0;
  /*int bytesRead = */ PHYSFS_readBytes(assetFile, buf, filesize);
  //std::cout << "AssetManager: " << filename << " (" << bytesRead << " bytes)" << std::endl;
  PHYSFS_close(assetFile);
  SDL_RWops* rw = SDL_RWFromConstMem(buf, filesize);
  return rw;
}

std::string AssetManager::GetLastError() {
  return std::to_string(*PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
}

