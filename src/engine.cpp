#include <cstdlib>
#include <iostream>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "engine.h"
#include "state.h"
#include "timer.h"

static const char* WINDOW_TITLE = "Slot Machine";
static const bool FULLSCREEN = false;
static const bool USE_OPENGL = false;
static const bool SHOW_FPS = false;
static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;

void Engine::Init(int argc, char** argv) {
  int sdl_flags = 0;
  if (FULLSCREEN) {
    sdl_flags += SDL_WINDOW_FULLSCREEN;
  }
  if (USE_OPENGL) {
    sdl_flags += SDL_WINDOW_OPENGL;
  }

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL Error: " << SDL_GetError();
  } else {
    window = SDL_CreateWindow(WINDOW_TITLE,
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              sdl_flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, 1024, 768);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }

  int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
  int img_init = IMG_Init(img_flags);
  if ((img_init & img_flags) != img_flags) {
    std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
  }

  audio = new SoundSystem();

  if (TTF_Init() != 0){
    std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
  }

  assets = new AssetManager();
  assets->Init(argv[0]);

  events = new EventManager();
  events->Init();
  events->SystemSignal.connect_member(this, &Engine::HandleEvent);

  running_ = true;
}

void Engine::HandleEvent(SystemEvent e) {
  switch (e) {
    case QUIT:
      Quit();
      break;
    case POP_STATE:
      PopState();
      break;
    default:
      break;
  }
}

void Engine::Cleanup() {
  while (!states_.empty()) {
    states_.back()->Cleanup();
    states_.pop_back();
  }

  assets->Cleanup();
  events->Cleanup();
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Engine::ChangeState(State* s) {
  if (!states_.empty()) {
    states_.back()->Cleanup();
    states_.pop_back();
  }
  states_.push_back(s);
  states_.back()->Init(this);
}

void Engine::PushState(State* s) {
  if (!states_.empty()) {
    states_.back()->Pause();
  }
  states_.push_back(s);
  states_.back()->Init(this);
}

void Engine::PopState() {
  if (!states_.empty()) {
    states_.back()->Cleanup();
    states_.pop_back();
  }
  if (!states_.empty()) {
    states_.back()->Resume();
  } else {
    running_ = false;
  }
}

void Engine::HandleEvents() {
  if (running_) {
    events->HandleEvents();
  }
}

void Engine::Update() {
  if (running_) {
    states_.back()->Update();
  }

  if (SHOW_FPS) {
    if (frameCount % 1000 == 0) {
      std::cout << "FPS:" << fps_ << std::endl;
    }
  }
}

void Engine::Draw() {
  if (running_) {
    states_.back()->Draw();
  }
  ++frameCount;
}

bool Engine::Running() {
  return running_;
}

void Engine::StartFPSTimer() {
  fpsTimer_.Start();
}

void Engine::UpdateFPS() {
  fps_ = frameCount / (fpsTimer_.GetTicks() / 1000.f);
}

void Engine::Quit() {
  running_ = false;
}
