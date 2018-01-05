#include "sprite_factory.h"

#include <SDL_image.h>
#include <iostream>

SpriteFactory::SpriteFactory(SDL_Renderer* renderer) : renderer_(renderer) {}

Sprite* SpriteFactory::Load(const char* filename, int w, int h) {
  if (textures_.count(filename) == 0) {
    SDL_Surface* s = IMG_Load(filename);
    if (s == NULL) {
      std::cout << IMG_GetError() << std::endl;
      return NULL;
    }
    SDL_SetColorKey(s, SDL_TRUE, SDL_MapRGB(s->format, 0xFF, 0XFF, 0XFF));
    SDL_Texture* t = SDL_CreateTextureFromSurface(renderer_, s);
    SDL_FreeSurface(s);

    textures_[filename] = t;
  }

  return new Sprite(textures_[filename], w, h); 
}
