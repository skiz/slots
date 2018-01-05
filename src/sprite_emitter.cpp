#include "sprite_emitter.h"
#include "rand_range.h"

SpriteEmitter::SpriteEmitter() {}

void SpriteEmitter::Init(SDL_Renderer* renderer) {
  renderer_ = renderer;
  sprite_factory_ = new SpriteFactory(renderer_);
}

void SpriteEmitter::Draw() {
  for (auto p : particles_) {
    Sprite* s = p->GetSprite();
    SDL_Texture* t = s->GetTexture();

    SDL_Rect r;
    if ((p->GetTTL() + p->GetFrameModSpeed()) % 5 == 0) {
      r = s->GetRect(s->GetNextFrame());
    } else {
      r = s->GetRect(s->GetFrame());
    }
    SDL_Rect pos{
      static_cast<int>(p->GetXLocation()),
      static_cast<int>(p->GetYLocation()),
      80,
      80
    };
    SDL_RenderCopyEx(renderer_, t, &r, &pos, p->GetAngleRotation(), NULL, SDL_FLIP_NONE);
  }
}

SpriteParticle* SpriteEmitter::AddParticle() {
  Sprite* s = sprite_factory_->Load("assets/main/images/coin3.png", 80, 80);
  s->SetFrame(RandRange::Int(0, s->LastFrame()));
  Vec2<float> pos(RandRange::Float(0, 1440), 900);
  Vec2<float> speed(RandRange::Float(-5.0f,5.0f),
                    RandRange::Float(4.0f,12.0f));

  SpriteParticle* p = new SpriteParticle(s, pos, speed);
  Vec2<double> angle(RandRange::Int(0, 359),
                     RandRange::Int(-10, 10));
  p->SetAngle(angle); 
  p->SetFrameModSpeed(RandRange::Int(0, 7));
  particles_.push_back(p); 
  return p;
}

void SpriteEmitter::SetMaxParticles(unsigned long num) {
  max_particles_ = num;
}

void SpriteEmitter::Update() {
  if (particles_.size() < max_particles_) {
    // support randombly creating a particle every N frames
    if (next_particle_ == 0) {
      AddParticle();
      next_particle_ = RandRange::Int(1, 2);
    } else {
      next_particle_--;
    }
  }

  std::vector<SpriteParticle*>::iterator i;
  for (i = particles_.begin(); i != particles_.end(); ++i) {
    if ((*i)->GetTTL() < 1) {
      i = particles_.erase(i);
      AddParticle();
    }
    (*i)->Update();
  }
}

void SpriteEmitter::Cleanup() {}

