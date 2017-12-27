#include "engine.h"
#include "big_win_state.h"
#include "SDL_ttf.h"
#include <iostream>

BigWinState BigWinState::state;

void BigWinState::Init(Engine* e) {
  engine_ = e;

  event_bind_ = engine_->events->SystemSignal.connect_member(this, &BigWinState::HandleEvent);
  std::cout << "Event: " << event_bind_ << std::endl;
  engine_->events->DisableBetting();

  StoreBackground();

  SDL_Surface* s = engine_->assets->LoadSurface("/main/images/bigwin.png");
  big_win_ = SDL_CreateTextureFromSurface(engine_->renderer, s);
  SDL_FreeSurface(s);

  amount_ = 0;
  total_ = 0;
  continues_ = 0;
  exiting_ = false;
  inc_amount_ = 10;

  font_ = TTF_OpenFont("assets/main/fonts/sans.ttf", 120);

  std::cout << "Initializing BigWin State" << std::endl;

  engine_->audio->ResumeMusic();
  engine_->audio->PlayMusic("assets/main/sound/winner2.ogg");
}

void BigWinState::HandleEvent(SystemEvent e) {
  if (exiting_) return;

  switch (e) {
    case CONTINUE:
      if (continues_ == 0) {
	engine_->audio->PlayMusic("assets/main/sound/winner3.ogg");
	// speed up counting
	inc_amount_ = 500;
      } else if (continues_ == 1) {
	// jump to last count
	amount_ = total_;
      } else {
	// take the winnings
	 exiting_ = true;
	 engine_->PopAsyncState();
      }
      continues_++;
      break;
    default:
      break;
  }
}

void BigWinState::StoreBackground() {
  /*
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Surface *ss = SDL_CreateRGBSurface(0, rw, rh, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  SDL_RenderReadPixels(engine_->renderer, NULL, SDL_PIXELFORMAT_ARGB8888, ss->pixels, ss->pitch);
  bg_ = SDL_CreateTextureFromSurface(engine_->renderer, ss);
  SDL_FreeSurface(ss);
  */
}

void BigWinState::Cleanup() {
  //if (event_bind_ != -1) {
   // engine_->events->SystemSignal.disconnect(event_bind_);
  //  event_bind_ = -1;
  //}
  engine_->events->EnableBetting();
}

void BigWinState::Pause() {
  engine_->audio->PauseMusic();
}

void BigWinState::Resume() {
  //engine_->audio->ResumeMusic();  
}

void BigWinState::Update() {
  total_ = engine_->accounting->Paid(); 
}

void BigWinState::Draw() {
  //SDL_SetRenderDrawColor(engine_->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
  //SDL_RenderClear(engine_->renderer);
  //SDL_RenderCopy(engine_->renderer, bg_, NULL, NULL);

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
  pos.w = size_;
  pos.h = size_;
  pos.x = rw / 2 - pos.w / 2;
  pos.y = rh / 2 - pos.h / 2;


  // Copy the texture to the renderer
  SDL_RenderCopy(engine_->renderer, big_win_, NULL, &pos);

  // Render the animated amount texture
  //
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





