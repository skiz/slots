#include "engine.h"
#include "big_win_state.h"
#include "SDL_ttf.h"
#include <iostream>
#include "main_state.h"

// TODO: doesn't increment winnings visually like pay_state
// TODO: replace numbers with bitmap images
// TODO: inherit pay state for standard stuff
BigWinState BigWinState::state;

void BigWinState::Init(Engine* e) {
  engine_ = e;

  engine_->events->DisableBetting();
  event_bind_ = engine_->events->SystemSignal.connect_member(this, &BigWinState::HandleEvent);

  SDL_Surface* s = engine_->assets->LoadSurface("/main/images/bigwin2.png");
  big_win_ = SDL_CreateTextureFromSurface(engine_->renderer, s);
  SDL_FreeSurface(s);

  amount_ = 0;
  total_ = 0;
  continues_ = 0;
  exiting_ = false;
  inc_amount_ = 10;

  font_ = engine_->assets->LoadFont("/main/fonts/sans.ttf", 120);

  engine_->audio->PlayMusic("/main/sound/winner2.ogg");
  engine_->audio->PlaySound("/main/sound/smooth_win.ogg", 2);

  coin_emitter_ = new SpriteEmitter();
  coin_emitter_->Init(engine_->renderer);

  // TODO: use asset manager
  texture_font_ = new TextureFont(engine_->renderer, "assets/main/fonts/cooper.ttf", 120);
  texture_font_->SetBackground("assets/main/images/sym_bg.png");
}

void BigWinState::HandleEvent(SystemEvent e) {
  switch (e) {
    case CONTINUE:
      continues_++; 
      if (continues_ == 1 && amount_ != total_) {
        // speed up counting
        engine_->audio->PlayMusic("/main/sound/winner3.ogg");
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
  engine_->audio->StopSound();
  coin_emitter_->Cleanup();
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
  coin_emitter_->Update();
}

void BigWinState::Draw() {
  const unsigned int credits = engine_->accounting->Credits() - total_ + amount_;
  MainState::Instance()->UpdatePaid(amount_);
  MainState::Instance()->UpdateCredits(credits);
  
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);

  if  (frame_ > 100){
    frame_ = 0;
  }

  if (frame_ >= 50) {
    dir_ = 0;
  } else {
    dir_ = 1;
  }

  if (dir_ == 0) {
    size_ -= 4;
  } else {
    size_ += 4;
  }

  SDL_Rect pos;
  int tw, th;
  SDL_QueryTexture(big_win_, NULL, NULL, &tw, &th);
  pos.w = tw * size_ / 300;
  pos.h = th * size_ / 300;
  pos.x = rw / 2 - pos.w / 2;
  pos.y = rh / 2 - pos.h / 2;

  coin_emitter_->Draw();

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

  int txw, txh;
  TTF_Font* f = texture_font_->GetFont();
  TTF_SizeText(f, text, &txw, &txh);

  SDL_Rect tpos;
  tpos.w = txw;
  tpos.h = txh;
  tpos.x = rw / 2 - tpos.w / 2;
  tpos.y = rh / 2 - tpos.h / 2 - 200;
  texture_font_->RenderText(text, tpos.x, tpos.y);

  ++frame_;
}





