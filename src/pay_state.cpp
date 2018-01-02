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

  engine_->audio->PlayMusic("assets/main/sound/winner2.ogg");
}

void PayState::HandleEvent(SystemEvent e) {
  switch (e) {
    case CONTINUE:
      continues_++; 
      if (continues_ == 1 && amount_ != total_) {
        engine_->audio->PlayMusic("assets/main/sound/winner3.ogg");
        inc_amount_ = 10;
      } else {
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
}

void PayState::Draw() {
  if (frame_ % frame_inc_ == 0 && amount_ < total_) {
    amount_ += inc_amount_;
  }
  if (amount_ >= total_) {
    amount_ = total_;
    engine_->audio->PauseMusic();
  }
  ++frame_;

  const unsigned int credits = engine_->accounting->Credits() - total_ + amount_;
  MainState::Instance()->UpdatePaid(amount_);
  MainState::Instance()->UpdateCredits(credits);

  if (amount_ == total_) {
    engine_->PopAsyncState();
  }

}
