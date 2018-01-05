#ifndef SPRITE_PARTICLE_H
#define SPRITE_PARTICLE_H

#include "vec2.h"
#include "sprite.h"
#include "particle.h"

class SpriteParticle : public Particle {
  public:
    SpriteParticle(Sprite* sprite, Vec2<float> loc, Vec2<float> speed)
    : Particle(loc, speed) {
      sprite_ = sprite;
    }

    Sprite* GetSprite() { return sprite_; }

  protected:
    Sprite* sprite_; // not owned

};

#endif

