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
  // Text notifications should probably be outside accounting in their own module...
  engine_->accounting->CreditUpdate.connect_member(this, &MainState::UpdateCredits);
  engine_->accounting->PaidUpdate.connect_member(this, &MainState::UpdatePaid);
  engine_->accounting->TextUpdate.connect_member(this, &MainState::UpdateText);
  engine_->accounting->BetUpdate.connect_member(this, &MainState::UpdateBet);
  engine_->accounting->TotalUpdate.connect_member(this, &MainState::UpdateTotal);
  engine_->accounting->LinesUpdate.connect_member(this, &MainState::UpdateLines);

  // request credits (TODO: use signals)
  UpdateCredits(engine_->accounting->Credits());
  UpdatePaid(engine_->accounting->Paid());
  UpdateText(engine_->accounting->Text());
  UpdateBet(engine_->accounting->Bet());
  UpdateTotal(engine_->accounting->Total());
  UpdateLines(engine_->accounting->Lines());
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

  SDL_Surface* s = engine_->assets->LoadSurface("/main/images/b.png");
  bg_ = SDL_CreateTextureFromSurface(engine_->renderer, s);
  SDL_FreeSurface(s);

  engine_->audio->PlayMusic("assets/main/sound/music.ogg");

  font_ = TTF_OpenFont("assets/main/fonts/sans.ttf", 40);
  credit_font_ = TTF_OpenFont("assets/main/fonts/digital.ttf", 65);

  SetupButtons();
}

void MainState::SetupButtons() {
  button_font_ = TTF_OpenFont("assets/main/fonts/sans.ttf", 20);
  button_font_color_ = {0,0,0,0}; //255, 255, 255, 0};


  int bx = 20;
  int by = 810;
  int bs = 150;

  const char* btnFile =  "/main/images/btn.png";

  cashBtn = new UIButton(engine_->renderer, btnFile);
  cashBtn->SetPosition(70, by);
  cashBtn->SetFont(button_font_, button_font_color_);
  cashBtn->SetText("CASHOUT");

  helpBtn = new UIButton(engine_->renderer, btnFile);
  helpBtn->SetPosition(220, by);
  helpBtn->SetFont(button_font_, button_font_color_);
  helpBtn->SetText("HELP");

  paysBtn = new UIButton(engine_->renderer, btnFile);
  paysBtn->SetPosition(220, by);
  paysBtn->SetFont(button_font_, button_font_color_);
  paysBtn->SetText("PAYLINES");

  linesBtn = new UIButton(engine_->renderer, btnFile);
  linesBtn->SetPosition(520, by);
  linesBtn->SetFont(button_font_, button_font_color_);
  linesBtn->SetText("LINES");

  betBtn = new UIButton(engine_->renderer, btnFile);
  betBtn->SetPosition(680, by);
  betBtn->SetFont(button_font_, button_font_color_);
  betBtn->SetText("BET");

  spinBtn = new UIButton(engine_->renderer, btnFile);
  spinBtn->SetPosition(1110, by);
  spinBtn->SetFont(button_font_, button_font_color_);
  spinBtn->SetText("SPIN");

  maxBtn = new UIButton(engine_->renderer, btnFile);
  maxBtn->SetPosition(1252, by);
  maxBtn->SetFont(button_font_, button_font_color_);
  maxBtn->SetText("BET MAX");

}

void MainState::Pause() {
  engine_->audio->PauseMusic();
}

void MainState::Resume() {
  engine_->audio->ResumeMusic();
}

void MainState::Update() {
	// This is where we will set up animations and reel contents
}

void MainState::RenderCredits() {
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect credit_pos;
  credit_pos.w = credit_width_;
  credit_pos.h = credit_height_;
  credit_pos.x = 270 - credit_width_;
  credit_pos.y = rh - credit_pos.h - 120;
  SDL_RenderCopy(engine_->renderer, credits_, NULL, &credit_pos);
}

void MainState::RenderBet() {
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect bet_pos;
  bet_pos.w = bet_width_;
  bet_pos.h = bet_height_;
  bet_pos.x = 755 - bet_width_;
  bet_pos.y = rh - bet_pos.h - 120;
  SDL_RenderCopy(engine_->renderer, bet_, NULL, &bet_pos);
}

void MainState::RenderLines() {
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect lines_pos;
  lines_pos.w = lines_width_;
  lines_pos.h = lines_height_;
  lines_pos.x = 600 - lines_width_;
  lines_pos.y = rh - lines_pos.h - 120;
  SDL_RenderCopy(engine_->renderer, lines_, NULL, &lines_pos);	
}

void MainState::Draw() {
  SDL_RenderCopy(engine_->renderer, bg_, NULL, NULL);
  RenderCredits();
  RenderPaid();
  RenderBet();
  RenderLines();
  RenderTotal();
  RenderMessageText();
  maxBtn->Render();
  betBtn->Render();
  spinBtn->Render();
  cashBtn->Render();
  linesBtn->Render();
  helpBtn->Render();
  //paysBtn->Render();
  SDL_RenderPresent(engine_->renderer);
  SDL_Delay(1);
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

void MainState::UpdateBet(const unsigned int &amount) {
  const char* text = std::to_string(amount).c_str();
  SDL_Surface* textSurface = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  TTF_SizeText(credit_font_, text, &bet_width_, &bet_height_);
  textSurface = TTF_RenderText_Blended(credit_font_, text, textColor);
  bet_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);
}

void MainState::UpdateLines(const unsigned int &amount) {
  const char* text = std::to_string(amount).c_str();
  SDL_Surface* textSurface = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  TTF_SizeText(credit_font_, text, &lines_width_, &lines_height_);
  textSurface = TTF_RenderText_Blended(credit_font_, text, textColor);
  lines_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);
}

void MainState::UpdateTotal(const unsigned int &amount) {
  const char* text = std::to_string(amount).c_str();
  SDL_Surface* textSurface = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  TTF_SizeText(credit_font_, text, &total_width_, &total_height_);
  textSurface = TTF_RenderText_Blended(credit_font_, text, textColor);
  total_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);
}

void MainState::RenderPaid() {
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect paid_pos;
  paid_pos.w = paid_width_;
  paid_pos.h = paid_height_;
  paid_pos.x = rw - paid_pos.w - 130;
  paid_pos.y = rh - paid_pos.h - 120;
  SDL_RenderCopy(engine_->renderer, paid_, NULL, &paid_pos);
}

void MainState::RenderTotal() {
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect total_pos;
  total_pos.w = total_width_;
  total_pos.h = total_height_;
  total_pos.x = rw - total_pos.w - 490;
  total_pos.y = rh - total_pos.h - 120;
  SDL_RenderCopy(engine_->renderer, total_, NULL, &total_pos);
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
  text_pos.y = 10;
  SDL_RenderCopy(engine_->renderer, text_, NULL, &text_pos);
}


