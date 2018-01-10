#include "engine.h"
#include "pay_state.h"
#include "SDL_ttf.h"
#include <iostream>
#include "main_state.h"

PayState PayState::state;

// TODO: Don't exit pay state directly, but store pressed key
// then replay key after exiting.  This will allow spinning directly
// from pay state and let animations run forever until button is pressed.


// WHAT IS A PAY STATE?
// Custom pay states are supported by extending this class.
// This includes states such as BigWin and Jackpot states.
// These types of states run in concert with the main state, however the main
// state will not accept any bet related events when running a pay state.
//
// HOW ARE WE GOING TO HANDLE BONUS/FREE GAMES?
// Main state should handle free spins within itself using the accounting system.
// When a free spin game is awarded, an async free spin state will be entered.
// It is for graphic/sound purposes only, and will exit to main state for the
// If there are free spins, main state will render "[X] FREE SPINS REMAINING",
// while using a different reel strategy.
//
// Rename MainState to BasicState?
// BonusGameState (we may want multiple bonus games based on symbols or randomly)
// FreeSpinState - Perhaps async over main state, showing "SPINS REMAINING" and playing
// music while free spins are going. This would also require changing odds and even
// perhaps available symbols/paytables.
//
// This may also require having extended reels for each type (cleaner/easier?)


// Main state seems to have nearly all of the code required except that we
// will want different symbol odds and routines.  A free spin state extends Main
// state and will provide the same game mechanics, but will 
//
//
// When a bonus is trigged, a new bonus state will be instantiated.
// It will 
//

// Basic pay related items that should always run such as updating the credits,
// playing credit updates and showing winning pay lines and animating the payline symbols.
//
// Different win states will be initiated by external code.
// Init() will be called each time the state is instantiated.
// Cleanup() will be called each time the state is exited.
//




void PayState::Init(Engine* e) {
  engine_ = e;

  engine_->events->DisableBetting();
  event_bind_ = engine_->events->SystemSignal.connect_member(this, &PayState::HandleEvent);

  amount_ = 0;
  total_ = 0;
  continues_ = 0;
  inc_amount_ = 1;
  reel_ = engine_->accounting->GetReel();
  num_wins_ = reel_->GetWinningPaylinePositions().size();
  engine_->audio->PlayMusic("/main/sound/winner2.ogg");
  engine_->audio->PlaySound("/main/sound/short_win.ogg", 2);

  SDL_Surface* ss = engine_->assets->LoadSurface("/reels/blank.png");
  blank_ = SDL_CreateTextureFromSurface(engine_->renderer, ss);
  SDL_FreeSurface(ss);

  winning_paylines_ = reel_->GetWinningLines();
}

void PayState::HandleEvent(SystemEvent e) {
  switch (e) {
    case CONTINUE:
      continues_++; 
      if (continues_ == 1 && amount_ != total_) {
        engine_->audio->PlayMusic("/main/sound/winner3.ogg");
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
  if (frame_ % 20 == 0) {
    // FUCK!
    payline_index_++; // this is the index of the winning line we want to show 
    if (payline_index_ >= num_wins_) {
      payline_index_ = 0;
    }

    //draw squares around winning paylines
    
  }
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
  // TODO: Use a timer to itererate winning paylines
  // Perhaps I should just add a MainState method to flash/reset...
  // cover up the winning line symbols to emulate flashing of winning symbols 
  SDL_Rect pos;

  //std::cout << show_win_line_ << std::endl;
  if(frame_ % 30 == 0) {
    // TODO: There is a possibility of hitting an out of bounds index here which isn't in
    // GetWinningPositionsForPayline
    for (auto s  : reel_->GetWinningPositionsForPayline(winning_paylines_[payline_index_])) {
      int column = s % 5;
      pos.w = 220;
      pos.h = 220;
      pos.x = 87 + column * 263;
      pos.y = 80 + s / 5 * 200;
      SDL_RenderCopy(engine_->renderer, blank_, NULL, &pos);
    }
  }
}


