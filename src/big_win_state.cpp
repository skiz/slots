#include "engine.h"
#include "big_win_state.h"
#include "SDL_ttf.h"

BigWinState BigWinState::state;

void BigWinState::Init(Engine* e) {
  engine_ = e;

  //engine_->events->SystemSignal.connect_member(this, &MainState::HandleEvent);
  engine_->events->DisableBetting();

  StoreBackground();

  SDL_Surface* s = engine_->assets->LoadSurface("/main/images/bigwin.png");
  big_win_ = SDL_CreateTextureFromSurface(engine_->renderer, s);
  SDL_FreeSurface(s);

  total_ = engine_->accounting->Paid(); 
  amount_ = 0;

  font_ = TTF_OpenFont("assets/main/fonts/sans.ttf", 120);
}

void BigWinState::StoreBackground() {
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Surface *ss = SDL_CreateRGBSurface(0, rw, rh, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  SDL_RenderReadPixels(engine_->renderer, NULL, SDL_PIXELFORMAT_ARGB8888, ss->pixels, ss->pitch);
  bg_ = SDL_CreateTextureFromSurface(engine_->renderer, ss);
  SDL_FreeSurface(ss);
}

void BigWinState::Cleanup() {
  engine_->events->EnableBetting();
}

void BigWinState::Pause() {
  engine_->audio->PauseMusic();  
}

void BigWinState::Resume() {
  engine_->audio->ResumeMusic();  
}

void BigWinState::Update() {
}

void BigWinState::Draw() {
  SDL_SetRenderDrawColor(engine_->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
  SDL_RenderClear(engine_->renderer);
  SDL_RenderCopy(engine_->renderer, bg_, NULL, NULL);

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
  if (frame_ % 5 == 0 && amount_ < total_) {
    amount_ += 10;
  }
  if (amount_ > total_) {
    amount_ = total_;
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

  //Update screen
  SDL_RenderPresent(engine_->renderer);

  ++frame_;
}





