#ifndef SPRITE_NUMBERS_H
#define SPRITE_NUMBERS_H

#include "asset_manager.h"

/*
 * Provides a utility to draw numbers using a sprite sheet.
 *
 * Example:
 *  int sw, sh = 30, 40; // per sprite
 *  SpriteNumbers sn(renderer, "path/to/numbers.png", sw, sh);
 *  sn.SetNumber(4500.24);
 *
 *  int nw, nh = 0;
 *  SDL_QueryTexture(sn.GetTexture(), 
 *  sn.Draw(SDL_Pos)
 *
 */
class SpriteNumbers {
  public:
    SpriteNumbers();
    ~SpriteNumbers();
  protected:
    AssetManager* assets_;
};

#endif
