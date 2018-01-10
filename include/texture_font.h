#ifndef TEXTURE_FONT_H
#define TEXTURE_FONT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <unordered_map>
#include "asset_manager.h"

/*
 * Simple TTF based rendering support for bitmapped texture fonts.
 *
 * Example:
 *
 *   int font_size = 40;
 *   tf = new TextureFont("path/to/font.ttf", font_size);
 *   tf->SetBackground("path/to/bg.png);
 *   tf->RenderText("something", 20, 20);  // x,y
 *   delete tf;
 *
 */

class TextureFont {
  public:
    TextureFont(SDL_Renderer* renderer, const char* font_path, int font_size);
    ~TextureFont();
    void SetBackground(const char* bg_path);
    TTF_Font* GetFont();
    void RenderText(const char* text, int x, int y);
  protected:
    Uint32 GetPixel32(SDL_Surface* surface, int x, int y);
    void PutPixel32(SDL_Surface* surface, int x, int y, Uint32 pixel);
    SDL_Texture* GenerateSymbol(const char& sym);
    void RenderSymbol(const char& sym, int x, int y);
    AssetManager* assets_;
    SDL_Renderer* renderer_;
    SDL_Surface* bg_surface_;
    TTF_Font *font_;
    std::unordered_map<char, SDL_Texture*> symbol_cache_;
    std::unordered_map<char, int> symbol_width_;
};

#endif

