#include <iostream>
#include "engine.h"
#include "main_state.h"
#include "sdl_ttf.h"

MainState MainState::state;

void MainState::Init(Engine* e) {
  engine_ = e;
  LoadAssets();

  // subscribe to system events
  // TODO: unsubscribe on pop/cleanup
  engine_->events->SystemSignal.connect_member(this, &MainState::HandleEvent);

  // handle credit updates
  engine_->accounting->CreditUpdate.connect_member(this, &MainState::UpdateCredits);

  //UpdateCredits(435234);
}

void MainState::Cleanup() {
  TTF_CloseFont(credit_font_);
}

void MainState::HandleEvent(SystemEvent e) {
  switch (e) {
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

  // TODO: Add font loading support to asset manager
  credit_font_ = TTF_OpenFont("assets/main/fonts/digital.ttf", 65);
}

void MainState::Pause() {
  engine_->audio->PauseMusic();
}

void MainState::Resume() {
  engine_->audio->ResumeMusic();
}

void MainState::Update() {}

void MainState::RenderCredits() {
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect credit_pos;
  int offset = 20;
  credit_pos.w = 220;
  credit_pos.h = 50;
  credit_pos.x = rw - credit_pos.w - offset;
  credit_pos.y = rh - credit_pos.h - offset;
  SDL_RenderCopy(engine_->renderer, credits_, NULL, &credit_pos);
}

void MainState::Draw() {
  SDL_RenderCopy(engine_->renderer, bg_, NULL, NULL);

  RenderCredits();

  SDL_RenderPresent(engine_->renderer);
  SDL_Delay(20);
}

void MainState::UpdateCredits(const unsigned int& amount) {
  SDL_Surface* text = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  text = TTF_RenderText_Solid(credit_font_, std::to_string(amount).c_str(), textColor);
  credits_ = SDL_CreateTextureFromSurface(engine_->renderer, text);
  SDL_FreeSurface(text);
}


