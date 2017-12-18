#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"


/**
 * Customizable UI Button
 */
class UIButton {
  public:
    UIButton(SDL_Renderer* renderer, const char* filename);
    void SetFont(TTF_Font* font, SDL_Color color);
    void SetPosition(int x, int y);
    void SetText(const char* text);
    void Render();
    ~UIButton() {};

  protected:
    void updateText();
    SDL_Texture* btn_texture_;
    SDL_Texture* txt_texture_;
    SDL_Rect btn_pos_;
    SDL_Rect text_pos_;
    TTF_Font* font_;
    SDL_Color font_color_;
    const char* text_;
    const char* filename_;
    SDL_Renderer* renderer_;
};

#endif
