#ifndef SPRITE_H
#define SPRITE_H

#include <SDL_image.h>

class Sprite {
  public:
    Sprite(SDL_Texture* t, int w, int h);
    SDL_Texture* GetTexture();
    SDL_Rect GetRect(int frame); // zero index
    int LastFrame();             // zero index
    int GetFrame();
    int GetNextFrame();
    void SetFrame(int frame);
  private:
    int tw_, th_, w_, h_;
    int current_frame_ = 0;
    SDL_Texture* texture_;       // not owned
};

#endif
