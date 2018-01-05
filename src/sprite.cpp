#include "sprite.h"

#include <SDL_image.h>
#include <iostream>

Sprite::Sprite(SDL_Texture* t, int w, int h)
  : w_(w), h_(h), texture_(t) {
    SDL_QueryTexture(texture_, NULL, NULL, &tw_, &th_);
    SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);
}

SDL_Texture* Sprite::GetTexture() {
  return texture_;
}

SDL_Rect Sprite::GetRect(int frame) {
  return SDL_Rect{frame * w_, 0, w_, h_};
}

int Sprite::LastFrame() {
  return tw_ / w_ - 1;
}

int Sprite::GetNextFrame() {
  if (current_frame_ >= LastFrame()) {
    current_frame_ = 0;
  } else {
    ++current_frame_;
  }
  return current_frame_;
}

void Sprite::SetFrame(int frame) {
  if (frame < 0) {
    current_frame_ = 0;
    return;
  } else if (frame > LastFrame()) {
    current_frame_ = LastFrame();
    return;
  }
  current_frame_ = frame;
  return;
}

int Sprite::GetFrame() {
  return current_frame_;
}
