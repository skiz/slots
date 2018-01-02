#include "engine.h"
#include "big_win_state.h"
#include "SDL_ttf.h"
#include <iostream>

BigWinState BigWinState::state;

void BigWinState::Init(Engine* e) {
  engine_ = e;

  engine_->events->DisableBetting();
  event_bind_ = engine_->events->SystemSignal.connect_member(this, &BigWinState::HandleEvent);

  SDL_Surface* s = engine_->assets->LoadSurface("/main/images/bigwin.png");
  big_win_ = SDL_CreateTextureFromSurface(engine_->renderer, s);
  SDL_FreeSurface(s);

  amount_ = 0;
  total_ = 0;
  continues_ = 0;
  exiting_ = false;
  inc_amount_ = 10;

  font_ = TTF_OpenFont("assets/main/fonts/sans.ttf", 120);

  engine_->audio->PlayMusic("assets/main/sound/winner2.ogg");
}

void BigWinState::HandleEvent(SystemEvent e) {
  switch (e) {
    case CONTINUE:
      continues_++; 
      if (continues_ == 1) {
	// speed up counting
	engine_->audio->PlayMusic("assets/main/sound/winner3.ogg");
	inc_amount_ = 500;
      } else if (continues_ == 2 && amount_ < total_) {
	// jump to last count
	amount_ = total_;
      } else if (amount_ == total_) {
	 // take the winnings
	 continues_ = 0;
	 inc_amount_ = 10;
	 amount_ = 0;
	 total_ = 0;
	 engine_->PopAsyncState();
	 return;
      }
      break;
    default:
      break;
  }
}

void BigWinState::Cleanup() {
  engine_->events->SystemSignal.disconnect(event_bind_);
  engine_->events->EnableBetting();
}

void BigWinState::Pause() {
  engine_->audio->PauseMusic();
}

void BigWinState::Resume() {
  engine_->audio->ResumeMusic();  
}

void BigWinState::Update() {
  total_ = engine_->accounting->Paid(); 
}

void BigWinState::Draw() {
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);

  int w, h;

  if  (frame_ > 100){
    frame_ = 0;
  }

  if (frame_ >= 50) {
    dir_ = 0;
  } else {
    dir_ = 1;
  }

  if (dir_ == 0) {
    size_ -= 10;
  } else {
    size_ += 10;
  }

  SDL_Rect pos;
  int tw, th;
  SDL_QueryTexture(big_win_, NULL, NULL, &tw, &th);
  pos.w = tw * size_ / 300;
  pos.h = th * size_ / 300;
  pos.x = rw / 2 - pos.w / 2;
  pos.y = rh / 2 - pos.h / 2;


  // Copy the texture to the renderer
  SDL_RenderCopy(engine_->renderer, big_win_, NULL, &pos);

  // Render the animated amount texture
  if (frame_ % frame_inc_ == 0 && amount_ < total_) {
    amount_ += inc_amount_;
  }
  if (amount_ >= total_) {
    amount_ = total_;
    engine_->audio->PauseMusic();
  }

  const char* text = std::to_string(amount_).c_str();
  SDL_Surface* textSurface = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  int txw, txh;
  TTF_SizeText(font_, text, &txw, &txh);
  textSurface = TTF_RenderText_Blended(font_, text, textColor);
  counter_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);

  SDL_Rect tpos;
  tpos.w = txw;
  tpos.h = txh;
  tpos.x = rw / 2 - tpos.w / 2;
  tpos.y = rh / 2 - tpos.h / 2 - 200;
  SDL_RenderCopy(engine_->renderer, counter_, NULL, &tpos);

  ++frame_;
}





