#include "sound_system.h"
#include "asset_manager.h"
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
  assets_ = &AssetManager::GetInstance();
}

SoundSystem::~SoundSystem() {
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
  return assets_->LoadSound(sound_path);
}

/*
void SoundSystem::PlaySound(std::string sound_path, int channel) {
  Mix_Chunk* snd = LoadSound(sound_path);
  if (snd) {
    Mix_PlayChannel(channel, snd, 0);
  }
}
*/

void SoundSystem::PlaySound(std::string sound_path, int channel, float speed) {
  Mix_Chunk* snd = LoadSound(sound_path);
  if (snd) {
    Mix_PlayChannel(channel, snd, 0);
  }
}

void SoundSystem::StopSound(int channel) {
  Mix_HaltChannel(channel);
}

Mix_Music* SoundSystem::LoadMusic(std::string& music_path) {
  return assets_->LoadMusic(music_path);
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


