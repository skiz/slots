#include <iostream>

#include "engine.h"
#include "main_state.h"

MainState MainState::state;

void MainState::Init(Engine* e) {
  engine_ = e;
  LoadAssets(); 

  // subscribe to system events
  engine_->events->SystemSignal.connect_member(this, &MainState::HandleEvent);
}

void MainState::Cleanup() {
}

void MainState::HandleEvent(SystemEvent e) {
  switch (e) {
    // TODO: This is a higher level thing than for a specific state, 
    // so it should probably be handled in the accounting subsystem.
    case COIN_IN:
      engine_->audio->PlaySound("assets/main/sound/coin.wav");
      break;
    default:
      break;
  }
}


void MainState::LoadAssets() {
  engine_->assets->Mount("assets/main", "/main");

  SDL_Surface* s = engine_->assets->LoadSurface("/main/images/background.png");
  bg_ = SDL_CreateTextureFromSurface(engine_->renderer, s);
  SDL_FreeSurface(s);

  // TODO: Audio system doesn't use Asset Manager currently
  engine_->audio->PlayMusic("assets/main/sound/music.ogg");
}

void MainState::Pause() {
  engine_->audio->PauseMusic();
}

void MainState::Resume() {
  engine_->audio->ResumeMusic();
}

void MainState::Update() {}

void MainState::Draw() {
  SDL_RenderCopy(engine_->renderer, bg_, NULL, NULL);
  SDL_RenderPresent(engine_->renderer);
  SDL_Delay(20);
}

