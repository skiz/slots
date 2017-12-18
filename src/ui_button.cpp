#include "ui_button.h"
#include "asset_manager.h"
#include "SDL.h"
#include "SDL_ttf.h"

UIButton::UIButton(SDL_Renderer* r, const char* f) : filename_(f), renderer_(r) {
    AssetManager& am = AssetManager::GetInstance();
    SDL_Surface* s = am.LoadSurface(f);
    btn_texture_ = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);
    SDL_QueryTexture(btn_texture_, NULL, NULL, &btn_pos_.w, &btn_pos_.h);
  }

void UIButton::SetPosition(int x, int y) {
  btn_pos_.x = x;
  btn_pos_.y = y;
}

void UIButton::SetFont(TTF_Font* font, SDL_Color color) {
  font_color_ = color;
  font_ = font;
}

void UIButton::SetText(const char* text) {
  text_ = text;
  if (font_ != nullptr) {
    updateText();
  }
}

void UIButton::Render() {
   SDL_RenderCopy(renderer_, btn_texture_, NULL, &btn_pos_);
   SDL_RenderCopy(renderer_, txt_texture_, NULL, &text_pos_);
}

void UIButton::updateText() {
  int txt_w, txt_h;
  SDL_Surface* textSurface = NULL;
  TTF_SizeText(font_, text_, &txt_w, &txt_h);
  textSurface = TTF_RenderText_Blended(font_, text_, font_color_);
  txt_texture_ = SDL_CreateTextureFromSurface(renderer_, textSurface);
  SDL_FreeSurface(textSurface);

  text_pos_.w = txt_w;
  text_pos_.h = txt_h;
  text_pos_.x = btn_pos_.x + (btn_pos_.w / 2) - (txt_w / 2);
  text_pos_.y = btn_pos_.y + (btn_pos_.h / 2) - (txt_h / 2);
}
