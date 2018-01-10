#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "asset_manager.h"
#include <unordered_map>
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>

class SoundSystem {
 public:
  SoundSystem();
  ~SoundSystem();
  void PlayMusic(std::string music_path);
  void PlaySound(std::string sound_path, int channel=-1);
  void StopSound(int channel=-1); 
  void PauseMusic();
  void ResumeMusic();
 private:
  bool Init();
  Mix_Music* LoadMusic(std::string& music_path);
  Mix_Chunk* LoadSound(std::string& sound_path);
  AssetManager* assets_;
};

#endif

