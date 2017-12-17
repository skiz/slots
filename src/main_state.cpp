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
  engine_->accounting->PaidUpdate.connect_member(this, &MainState::UpdatePaid);

  // Text notifications should probably be outside accounting in their own module...
  engine_->accounting->TextUpdate.connect_member(this, &MainState::UpdateText);

  // request credits (TODO: use signals)
  UpdateCredits(engine_->accounting->Credits());
  UpdatePaid(engine_->accounting->Paid());
  UpdateText(engine_->accounting->Text());
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

  SDL_Surface* s = engine_->assets->LoadSurface("/main/images/background.jpg");
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
  credit_pos.w = credit_width_;
  credit_pos.h = credit_height_;
  credit_pos.x = 285 - credit_width_; //250; //rw - credit_pos.w - 30;
  credit_pos.y = rh - credit_pos.h - 25;
  SDL_RenderCopy(engine_->renderer, credits_, NULL, &credit_pos);
}

void MainState::Draw() {
  SDL_RenderCopy(engine_->renderer, bg_, NULL, NULL);
  RenderCredits();
  RenderPaid();
  RenderText();
  SDL_RenderPresent(engine_->renderer);
  SDL_Delay(20);
}

void MainState::UpdateCredits(const unsigned int &amount) {
  const char* text = std::to_string(amount).c_str();
  SDL_Surface* textSurface = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  TTF_SizeText(credit_font_, text, &credit_width_, &credit_height_);
  textSurface = TTF_RenderText_Solid(credit_font_, text, textColor);
  credits_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);
}

void MainState::UpdatePaid(const unsigned int &amount) {
  const char* text = std::to_string(amount).c_str();
  SDL_Surface* textSurface = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  TTF_SizeText(credit_font_, text, &paid_width_, &paid_height_);
  textSurface = TTF_RenderText_Solid(credit_font_, text, textColor);
  paid_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);
}

void MainState::RenderPaid() {
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect paid_pos;
  paid_pos.w = paid_width_;
  paid_pos.h = paid_height_;
  paid_pos.x = rw - paid_pos.w - 30;
  paid_pos.y = rh - paid_pos.h - 25;
  SDL_RenderCopy(engine_->renderer, paid_, NULL, &paid_pos);
}

void MainState::UpdateText(const char* text) {
  SDL_Surface* textSurface = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  TTF_SizeText(credit_font_, text, &text_width_, &text_height_);
  textSurface = TTF_RenderText_Solid(credit_font_, text, textColor);
  text_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);
}

void MainState::RenderText() {
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect text_pos;
  text_pos.w = text_width_;
  text_pos.h = text_height_;
  text_pos.x = rw/2 - text_width_/2;
  text_pos.y = rh - text_pos.h - 135;
  SDL_RenderCopy(engine_->renderer, text_, NULL, &text_pos);
}


