#include "sound_system.h"
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>

static const int AUDIO_FREQUENCY = 44100;
static const int AUDIO_CHANNELS = 2;
static const int AUDIO_SAMPLE_SIZE = 2048;

SoundSystem::SoundSystem() {
  if (!Init()) {
    std::cerr << "Unable to initialize sound system." << std::endl;
  }
}

SoundSystem::~SoundSystem() {
  for (auto& snd : sounds_) {
    Mix_FreeChunk(snd.second);
  }
  for (auto& m : music_) {
    Mix_FreeMusic(m.second);
  }
  music_.clear();
  sounds_.clear();
  Mix_Quit();
}

bool SoundSystem::Init() {
  if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
    std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
    return false;
  }

  if (Mix_OpenAudio(AUDIO_FREQUENCY, MIX_DEFAULT_FORMAT,
                    AUDIO_CHANNELS, AUDIO_SAMPLE_SIZE) < 0 ) {
    std::cerr << "SDL_mixer: " << Mix_GetError() << std::endl;
  }

  int mix_flags = MIX_INIT_OGG;
  int mix_init = Mix_Init(mix_flags);
  if ((mix_init & mix_flags) != mix_flags) {
    std::cerr << "Mix_Init: " << Mix_GetError() << std::endl;
    return false;
  }
  return true;
}

Mix_Chunk* SoundSystem::LoadSound(std::string& sound_path) {
  Mix_Chunk* snd;
  if (sounds_.count(sound_path) > 0){
    snd = sounds_[sound_path];
  } else {
    snd = Mix_LoadWAV(sound_path.c_str());
    if (!snd) {
      std::cerr << "Mix_LoadWav: " << Mix_GetError() << std::endl;
      return nullptr;
    }
    sounds_[sound_path] = snd;
  }
  return snd;
}

void SoundSystem::PlaySound(std::string sound_path, int channel) {
  Mix_Chunk* snd = LoadSound(sound_path);
  if (snd) {
    Mix_PlayChannel(channel, snd, 0);
  }
}

Mix_Music* SoundSystem::LoadMusic(std::string& music_path) {
  Mix_Music* music;
  if (music_.count(music_path) > 0) {
    music = music_[music_path];
  } else {
    music = Mix_LoadMUS(music_path.c_str());
    if (!music) {
      std::cerr << "Mix_LoadMUS: " << Mix_GetError() << std::endl;
      return nullptr;
    }
    music_[music_path] = music;
  }
  return music;
}

void SoundSystem::PlayMusic(std::string music_path) {
  Mix_Music* music = LoadMusic(music_path);
  if (music) {
    Mix_PlayMusic(music, -1);
  }
  ResumeMusic();
  
}

void SoundSystem::PauseMusic() {
    Mix_PauseMusic();
}

void SoundSystem::ResumeMusic() {
  Mix_ResumeMusic();
}


