#ifndef SPRITE_EMITTER_H
#define SPRITE_EMITTER_H

#include <vector>

#include "sprite_particle.h"
#include "sprite_factory.h"

class SpriteEmitter {
  public:
    void Init(SDL_Renderer* renderer);
    void Draw();
    void Update();
    void Cleanup();
    SpriteEmitter();

    void SetMaxParticles(unsigned long num);
    SpriteParticle* AddParticle();
  protected:
    std::vector<SpriteParticle*> particles_;
    SpriteFactory* sprite_factory_;
    SDL_Renderer* renderer_;  // not owned
    int next_particle_ = 0;
    unsigned long max_particles_ = 150l;
};

#endif
