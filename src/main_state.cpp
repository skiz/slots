#include <iostream>
#include "engine.h"
#include "main_state.h"
#include "SDL_ttf.h"

MainState MainState::state;

void MainState::Init(Engine* e) {
  engine_ = e;
  LoadAssets();

  // subscribe to system events
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
  TTF_CloseFont(font_);
  // TODO: disconnect slots
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

  engine_->audio->PlayMusic("assets/main/sound/music.ogg");

  font_ = TTF_OpenFont("assets/main/fonts/sans.ttf", 40);
  credit_font_ = TTF_OpenFont("assets/main/fonts/digital.ttf", 65);

  SetupButtons();
}

void MainState::SetupButtons() {
  button_font_ = TTF_OpenFont("assets/main/fonts/sans.ttf", 30);
  button_font_color_ = {255, 255, 255, 0};


  int bx = 755;
  int by = 580;
  int bs = 170;

  const char* btnFile =  "/main/images/green_button.png";

  maxBtn = new UIButton(engine_->renderer, btnFile);
  maxBtn->SetPosition(bx, by);
  maxBtn->SetFont(button_font_, button_font_color_);
  maxBtn->SetText("Bet Max");

  betBtn = new UIButton(engine_->renderer, btnFile);
  betBtn->SetPosition(bx+bs, by);
  betBtn->SetFont(button_font_, button_font_color_);
  betBtn->SetText("Bet 1");

  cashBtn = new UIButton(engine_->renderer, btnFile);
  cashBtn->SetPosition(bx+bs*2, by);
  cashBtn->SetFont(button_font_, button_font_color_);
  cashBtn->SetText("Cashout");

  spinBtn = new UIButton(engine_->renderer, btnFile);
  spinBtn->SetPosition(bx+bs*3, by);
  spinBtn->SetFont(button_font_, button_font_color_);
  spinBtn->SetText("Spin");
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
  RenderMessageText();
  maxBtn->Render();
  betBtn->Render();
  spinBtn->Render();
  cashBtn->Render();
  SDL_RenderPresent(engine_->renderer);
  //SDL_Delay(10);
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

void MainState::RenderMessageText() {
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect text_pos;
  text_pos.w = text_width_;
  text_pos.h = text_height_;
  text_pos.x = rw/2 - text_width_/2;
  text_pos.y = rh - text_pos.h - 135;
  SDL_RenderCopy(engine_->renderer, text_, NULL, &text_pos);
}


