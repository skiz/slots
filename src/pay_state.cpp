#include "engine.h"
#include "pay_state.h"
#include "SDL_ttf.h"
#include <iostream>
#include "main_state.h"

PayState PayState::state;

void PayState::Init(Engine* e) {
  engine_ = e;

  engine_->events->DisableBetting();
  event_bind_ = engine_->events->SystemSignal.connect_member(this, &PayState::HandleEvent);

  amount_ = 0;
  total_ = 0;
  continues_ = 0;
  inc_amount_ = 1;
  reel_ = engine_->accounting->GetReel();
  //num_wins_ = reel_->GetWinningPaylinePositions().size();
  engine_->audio->PlayMusic("assets/main/sound/winner2.ogg");

  /*
  SDL_Surface* ss = engine_->assets->LoadSurface("/reels/blank.png");
  blank_ = SDL_CreateTextureFromSurface(engine_->renderer, ss);
  SDL_FreeSurface(ss);

  winning_paylines_ = reel_->GetWinningLines();
  */
}

void PayState::HandleEvent(SystemEvent e) {
  switch (e) {
    case CONTINUE:
      continues_++; 
      if (continues_ == 1 && amount_ != total_) {
        engine_->audio->PlayMusic("assets/main/sound/winner3.ogg");
        inc_amount_ = 10;
      } else if (amount_ < total_) {
        amount_ = total_;
      }
      break;
    default:
      break;
  }
}

void PayState::Cleanup() {
   amount_ = 0;
   total_ = 0;
   continues_ = 0;
   inc_amount_ = 1;

  engine_->events->SystemSignal.disconnect(event_bind_);
  engine_->events->EnableBetting();
}

void PayState::Pause() {
  engine_->audio->PauseMusic();
}

void PayState::Resume() {
  engine_->audio->ResumeMusic();  
}

void PayState::Update() {
  total_ = engine_->accounting->Paid();

  if (frame_ % frame_inc_ == 0 && amount_ < total_) {
    amount_ += inc_amount_;
  }
  if (amount_ >= total_) {
    amount_ = total_;
    engine_->audio->PauseMusic();
  }

  // N frames per winning line shown
  // but we still need to show what was won for each line too... ugh.
  // this show_line_win is an index, but we have a map based on payline
  //
  /*
  if (frame_ % 20 == 0) {
    // FUCK!
    payline_index_++; // this is the index of the winning line we want to show 
    if (payline_index_ >= num_wins_) {
      payline_index_ = 0;
    }
  }
  */
  ++frame_;
}

void PayState::Draw() {
  const unsigned int credits = engine_->accounting->Credits() - total_ + amount_;
  MainState::Instance()->UpdatePaid(amount_);
  MainState::Instance()->UpdateCredits(credits);

  DrawPaylines();

  // Force exit when done
  if (amount_ == total_) {
    engine_->PopAsyncState();
  }
}

void PayState::DrawPaylines(){
  // Perhaps I should just add a MainState method to flash/reset...
  // cover up the winning line symbols to emulate flashing of winning symbols 
  //SDL_Rect pos;

  //std::cout << show_win_line_ << std::endl;
  // if(frames_ % 10 == 0) {.. //FLASH!.
  /*
  for (auto s  : reel_->GetWinningPositionsForPayline(winning_paylines_[payline_index_])) {
    int column = s % 5;
    pos.w = 220;
    pos.h = 220;
    pos.x = 87 + column * 263;
    pos.y = 80 + s / 5 * 200;
    SDL_RenderCopy(engine_->renderer, blank_, NULL, &pos);
  }
  */
}


