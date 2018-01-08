#include "texture_font.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

TextureFont::TextureFont(SDL_Renderer* renderer, const char* font_path, int font_size)
  : renderer_(renderer) {
    font_ = TTF_OpenFont(font_path, font_size);
    if (font_ == NULL) {
      std::cout << SDL_GetError() << std::endl;
      exit(1);
    }
  }

TextureFont::~TextureFont() {
  TTF_CloseFont(font_);
  SDL_FreeSurface(bg_surface_);

  for (auto s : symbol_cache_) {
    SDL_DestroyTexture(s.second);
  }
}

void TextureFont::SetBackground(const char* bg_path) {
  bg_surface_ = IMG_Load(bg_path);
}

TTF_Font* TextureFont::GetFont() {
  return font_;
}

void TextureFont::RenderText(const char* text, int x, int y) {
  int w, h;
  int i = 0;
  while (text[i] != 0) {
    const char c = text[i];
    if (symbol_cache_.count(c) == 0) {
      symbol_cache_[c] = GenerateSymbol(c);
    }
    SDL_QueryTexture(symbol_cache_[c], NULL, NULL, &w, &h);
    RenderSymbol(c, x, y);
    i++;
    x += w;
  }
}

SDL_Texture* TextureFont::GenerateSymbol(const char &sym) {
  SDL_Color textColor = {255, 0, 255, 0};
  int tw, th;
  TTF_SizeText(font_, &sym, &tw, &th); 
  char val[2] = {sym, 0};
  SDL_Surface* textSurface = TTF_RenderText_Blended(font_, val, textColor);
  SDL_Surface* formattedSurface = SDL_CreateRGBSurfaceWithFormat(0, textSurface->w, th, 32, SDL_PIXELFORMAT_RGBA32);

  // Copy resized background image to formattedSurface
  SDL_Rect bg_rect = {0, 0, bg_surface_->w, bg_surface_->h};
  SDL_Rect text_rect = {0, 0, tw, textSurface->h};
  SDL_BlitScaled(bg_surface_, &bg_rect, formattedSurface, &text_rect);

  // Update pixels based on formattedSurface RGB and textSurface Alpha.
  Uint32 pixel, bg_pixel = 0;
  Uint8 tr, tg, tb, ta;
  Uint8 br, bg, bb, ba;
  for (int x = 0; x < textSurface->w; x++) {
    for (int y = 0; y < textSurface->h; y++) {
      pixel = GetPixel32(textSurface, x, y);
      SDL_GetRGBA(pixel, textSurface->format, &tr, &tg, &tb, &ta);
      bg_pixel = GetPixel32(formattedSurface, x, y);
      SDL_GetRGBA(bg_pixel, formattedSurface->format, &br, &bg, &bb, &ba);
      pixel = SDL_MapRGBA(formattedSurface->format, br, bg, bb, ta);
      PutPixel32(formattedSurface, x, y, pixel);
    }
  }

  SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer_, formattedSurface);
  SDL_FreeSurface(textSurface);
  SDL_FreeSurface(formattedSurface);

  symbol_cache_[sym] = textTexture;
  return symbol_cache_[sym];
}

void TextureFont::RenderSymbol(const char &sym, int x, int y) {
  int tw, th;
  SDL_QueryTexture(symbol_cache_[sym], NULL, NULL, &tw, &th);
  SDL_Rect tpos;
  tpos.w = tw;
  tpos.h = th;
  tpos.x = x;
  tpos.y = y;
  SDL_RenderCopy(renderer_, symbol_cache_[sym], NULL, &tpos);
}

Uint32 TextureFont::GetPixel32(SDL_Surface *surface, int x, int y) {
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 4; //bpp;
  return *(Uint32 *)p;
}

void TextureFont::PutPixel32(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
    *(Uint32 *)p = pixel;
}

