#ifndef SPRITE_FACTORY_H
#define SPRITE_FACTORY_H

#include "sprite.h"
#include <SDL_image.h>
#include <unordered_map>

class SpriteFactory {
  public:
    SpriteFactory(SDL_Renderer* renderer);
    Sprite* Load(const char* filename, int w, int h);
  private:
    std::unordered_map<const char*, SDL_Texture*> textures_;
    SDL_Renderer* renderer_; // not owned
};

#endif
