#include <iostream>
#include "engine.h"
#include "main_state.h"
#include "SDL_ttf.h"
#include "pay_state.h"
#include "big_win_state.h"
#include "signal.h"

MainState MainState::state;

void MainState::Init(Engine* e) {
  engine_ = e;
  LoadAssets();

  reel_ = engine_->accounting->GetReel();

  reel_->GenerateSymbols(5, 3);

  // subscribe to system events
  //engine_->events->SystemSignal.connect_member(this, &MainState::HandleEvent);

  engine_->accounting->CreditUpdate.connect_member(this, &MainState::UpdateCredits);
  engine_->accounting->PaidUpdate.connect_member(this, &MainState::UpdatePaid);
  engine_->accounting->TextUpdate.connect_member(this, &MainState::UpdateText);
  engine_->accounting->BetUpdate.connect_member(this, &MainState::UpdateBet);
  engine_->accounting->TotalUpdate.connect_member(this, &MainState::UpdateTotal);
  engine_->accounting->LinesUpdate.connect_member(this, &MainState::UpdateLines);
  engine_->accounting->ReelsUpdate.connect_member(this, &MainState::UpdateReels);
  engine_->accounting->BigWin.connect_member(this, &MainState::BigWin);
  engine_->accounting->Win.connect_member(this, &MainState::Win);
  engine_->accounting->SpinStarted.connect_member(this, &MainState::SpinStarted);
  engine_->accounting->SpinStopped.connect_member(this, &MainState::SpinStopped);


  // request credits (TODO: use signals, these should be pushed to the state)
  UpdateCredits(engine_->accounting->Credits());
  UpdatePaid(engine_->accounting->Paid());
  UpdateText(engine_->accounting->Text());
  UpdateBet(engine_->accounting->Bet());
  UpdateTotal(engine_->accounting->Total());
  UpdateLines(engine_->accounting->Lines());

  engine_->events->EnableBetting();

  for(int i = 0; i < 5; ++i) {
    spinning_[i] = false;
    vertical_offset_[i] = 0;
  }

}

void MainState::Cleanup() {
  TTF_CloseFont(credit_font_);
  TTF_CloseFont(font_);

  for (auto s : signal_bindings_) {
    engine_->events->SystemSignal.disconnect(s);
  }
}

void MainState::HandleEvent(SystemEvent e) {
  switch (e) {
    case UPDATE_REELS:
      std::cout << "UPDATE REELS!" << std::endl;
    default:
      break;
  }
}

void MainState::SpinStarted() {
  engine_->audio->PlaySound("assets/main/sound/spin.wav");
  engine_->audio->PlayMusic("assets/main/sound/reels.wav");
  engine_->audio->ResumeMusic();
  for (int i = 0; i < 5; i++) {
    spinning_[i] = true;
    ScheduleStop(i, 2000 + i * 200);
  }
}

void MainState::SpinStopped() {
  engine_->audio->PauseMusic();
  for (int i = 0; i < 5; i++) {
    StopNext();
  }
}

void MainState::ScheduleStop(int col, int ms) {
  stop_timer_[col] = SDL_AddTimer(ms, &MainState::StopColumn, this);
}

Uint32 MainState::StopColumn(Uint32 /*interval*/, void *param) {
  ((MainState *) param)->StopNext();
  return 0L;
}

// Stops the first non-stopped column
void MainState::StopNext() {
  for (int i = 0; i < 5; i++) {
    if (spinning_[i]) {
      spinning_[i] = false;
      SDL_RemoveTimer(stop_timer_[i]);
      engine_->audio->PlaySound("assets/main/sound/spin.wav");
      if (i==4) {
	engine_->events->SystemSignal.emit(REELS_STOPPED);
      }
      return;
    }
  }
}

void MainState::LoadAssets() {
  engine_->assets->Mount("assets/main", "/main");

  SDL_Surface* ss = engine_->assets->LoadSurface("/main/images/b.png");
  bg_ = SDL_CreateTextureFromSurface(engine_->renderer, ss);
  SDL_FreeSurface(ss);

  // Add gray overlay texture
  SDL_Surface* s;
  s = SDL_CreateRGBSurface(0, 2000, 2000, 32, 0, 0, 0, 0);
  SDL_Rect rect = {0,0,2000,2000};
  SDL_FillRect(s, &rect, SDL_MapRGB(s->format, 205, 203, 206));
  gray_ = SDL_CreateTextureFromSurface(engine_->renderer, s);
  SDL_FreeSurface(s);

  font_ = TTF_OpenFont("assets/main/fonts/sans.ttf", 40);
  credit_font_ = TTF_OpenFont("assets/main/fonts/digital.ttf", 65);

  LoadReelSymbols();

  SetupButtons();
}

void MainState::LoadReelSymbols() {
  engine_->assets->Mount("assets/reels", "/reels");
  LoadSymbol(CHERRY, "/reels/cherry.png");
  LoadSymbol(BAR, "/reels/bar.png");
  LoadSymbol(DOUBLE_BAR, "/reels/bars.png");
  LoadSymbol(TEN, "/reels/banana.png");
  LoadSymbol(JACK, "/reels/heart.png");
  LoadSymbol(QUEEN, "/reels/crown.png");
  LoadSymbol(KING, "/reels/ruby.png");
  LoadSymbol(ACE, "/reels/diamond.png");
  LoadSymbol(WILD, "/reels/clover.png");
  LoadSymbol(BONUS, "/reels/bonus.png");
  LoadSymbol(JACKPOT, "/reels/bigwin.png");
  LoadSymbol(ALT1, "/reels/gem.png");
  LoadSymbol(ALT2, "/reels/money.png");
  LoadSymbol(ALT3, "/reels/lemon.png");
  LoadSymbol(ALT4, "/reels/orange.png");
  LoadSymbol(ALT5, "/reels/melon.png");
  LoadSymbol(ALT6, "/reels/sberry.png");
  LoadSymbol(ALT7, "/reels/bell.png");
  LoadSymbol(ALT8, "/reels/grape.png");
  LoadSymbol(ALT9, "/reels/melon2.png");
}

void MainState::RenderPaylineOverlay(int /*line*/) {
  // render a payline based on it's number on the reel
  /*
  int xoff = 50; // x offset
  int yoff = 50; // y offset
  int xinc = 200; // x increment
  int yinc = 200; // y increment
  std::array<std::array<int, 2>, 5> coords;
  */
  //for (auto p : reel_->GetPaylines()[line]) {
    // locate the 5 coordinates
    //coords.push_back({p}) 
  //}
}

void MainState::LoadSymbol(Symbol type, const char* filename) {
  SDL_Surface* s = engine_->assets->LoadSurface(filename);
  reel_symbols_[type] = SDL_CreateTextureFromSurface(engine_->renderer, s);
  SDL_FreeSurface(s);
}


void MainState::SetupButtons() {
  button_font_ = TTF_OpenFont("assets/main/fonts/sans.ttf", 20);
  button_font_color_ = {0,0,0,0}; //255, 255, 255, 0};

  int by = 810;

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

void MainState::StartSpin() {
  for (int i = 0; i < 5; i++) {
    spinning_[i] = true;
  }
}

void MainState::StopReels() {
  for (int i = 0; i < 5; i++) {
    spinning_[i] = false;
  }
}

void MainState::Pause() {
  engine_->audio->PauseMusic();
  engine_->events->DisableBetting();
}

void MainState::Resume() {
  engine_->events->EnableBetting();
}

// Reels were updated, lets render them.
void MainState::UpdateReels() {
}

void MainState::BigWin(const unsigned int& /*amount*/) {
  engine_->PushAsyncState(BigWinState::Instance());
}

void MainState::Win(const unsigned int& /*amount*/) {
  engine_->PushAsyncState(PayState::Instance());
}

void MainState::Update() {
  // This is where we will set up animations
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
  SDL_RenderCopy(engine_->renderer, gray_, NULL, NULL);
  RenderSymbols();
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

// DEPRECATED: Moved to pay state.
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

void MainState::RenderSymbols() {
  // We only have the final symbols at the moment, so just render those.
  int max_height = 220 * 3;

  SDL_Rect pos;

  for (int c = 0; c < 5; c++) {
    if (spinning_[c]) {
      vertical_offset_[c] += spin_speed_;
      if (vertical_offset_[c] >= max_height) {
	vertical_offset_[c] = 0;
      }
    } else {
      vertical_offset_[c] = 0;
    }
  }

  for (auto s : reel_->GetSymbols()) {
    int column = s.first % 5;
    pos.w = 220;
    pos.h = 220;
    pos.x = 87 + column * 263;
    pos.y = 80 + s.first / 5 * 200;
    pos.y = pos.y + vertical_offset_[column];
    SDL_RenderCopy(engine_->renderer, reel_symbols_[s.second], NULL, &pos);
    pos.y = pos.y - max_height;
    SDL_RenderCopy(engine_->renderer, reel_symbols_[s.second], NULL, &pos);
  }
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


