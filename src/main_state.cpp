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
  
  UpdateSpinText();
}

void MainState::Cleanup() {
  TTF_CloseFont(credit_font_);
  TTF_CloseFont(font_);
}

void MainState::HandleEvent(SystemEvent e) {
  switch (e) {
    default:
      break;
  }
}

void MainState::UpdateSpinText() {
  const char* text = "SPIN";
  SDL_Surface* textSurface = NULL;
  SDL_Color textColor = {255, 255, 255, 50};
  TTF_SizeText(font_, text, &spin_text_width_, &spin_text_height_);
  textSurface = TTF_RenderText_Blended(font_, text, textColor);
  spin_text_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);
}

void MainState::RenderCashBtn() {
  int w, h;

  int tx = 260;
  int ty = 530;

  SDL_QueryTexture(cash_btn_, NULL, NULL, &w, &h);
  SDL_Rect cash_pos;
  cash_pos.w = w;
  cash_pos.h = h;
  cash_pos.x = tx;
  cash_pos.y = ty;
  SDL_RenderCopy(engine_->renderer, cash_btn_, NULL, &cash_pos);
}

void MainState::RenderMaxBtn() {
  int w, h;

  int tx = 1000;
  int ty = 580;

  SDL_QueryTexture(max_btn_, NULL, NULL, &w, &h);
  SDL_Rect max_pos;
  max_pos.w = w;
  max_pos.h = h;
  max_pos.x = tx;
  max_pos.y = ty;
  SDL_RenderCopy(engine_->renderer, max_btn_, NULL, &max_pos);

}

void MainState::RenderSpinBtn() {
  int w, h;

  int tx = 1200;
  int ty = 580;

  SDL_QueryTexture(spin_btn_, NULL, NULL, &w, &h);
  SDL_Rect spin_pos;
  spin_pos.w = w;
  spin_pos.h = h;
  spin_pos.x = tx;
  spin_pos.y = ty;

  SDL_Rect text_pos;
  text_pos.w = spin_text_width_;
  text_pos.h = spin_text_height_;
  text_pos.x = tx + w - (w / 2) - spin_text_width_ / 2 ;
  text_pos.y = ty + h - (h / 2) - spin_text_height_ / 2;

  SDL_RenderCopy(engine_->renderer, spin_btn_, NULL, &spin_pos);
  SDL_RenderCopy(engine_->renderer, spin_text_, NULL, &text_pos);
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
  font_ = TTF_OpenFont("assets/main/fonts/sans.ttf", 50);

  s = engine_->assets->LoadSurface("/main/images/green_button.png");
  spin_btn_ = SDL_CreateTextureFromSurface(engine_->renderer, s);
  SDL_FreeSurface(s);

  s = engine_->assets->LoadSurface("/main/images/red_button.png");
  max_btn_ = SDL_CreateTextureFromSurface(engine_->renderer, s);
  SDL_FreeSurface(s);

  s = engine_->assets->LoadSurface("/main/images/cashout.png");
  cash_btn_ = SDL_CreateTextureFromSurface(engine_->renderer, s);
  SDL_FreeSurface(s);
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
  RenderSpinBtn();
  RenderMaxBtn();
  RenderCashBtn();
  SDL_RenderPresent(engine_->renderer);
  SDL_Delay(20);
}

void MainState::UpdateCredits(const unsigned int &amount) {
  const char* text = std::to_string(amount).c_str();
  SDL_Surface* textSurface = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  TTF_SizeText(credit_font_, text, &credit_width_, &credit_height_);
  textSurface = TTF_RenderText_Blended(credit_font_, text, textColor);
  credits_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);
}

void MainState::UpdatePaid(const unsigned int &amount) {
  const char* text = std::to_string(amount).c_str();
  SDL_Surface* textSurface = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  TTF_SizeText(credit_font_, text, &paid_width_, &paid_height_);
  textSurface = TTF_RenderText_Blended(credit_font_, text, textColor);
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
  SDL_Color textColor = {255, 255, 255, 0};
  TTF_SizeText(font_, text, &text_width_, &text_height_);
  textSurface = TTF_RenderText_Blended(font_, text, textColor);
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


