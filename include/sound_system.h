#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

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
  void PauseMusic();
  void ResumeMusic();
 private:
  bool Init();
  Mix_Music* LoadMusic(std::string& music_path);
  Mix_Chunk* LoadSound(std::string& sound_path);
  std::unordered_map<std::string, Mix_Chunk*> sounds_;
  std::unordered_map<std::string, Mix_Music*> music_;
};

#endif

