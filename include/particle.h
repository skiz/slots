#ifndef PARTICLE_H
#define PARTICLE_H

#include "vec2.h"

class Particle {
  public:
    Particle(Vec2<float> loc) {
      location_ = loc;
      Init();
    }

    Particle(Vec2<float> loc, Vec2<float> speed) {
      location_ = loc;
      speed_ = speed;
      Init();
    }

    void Init() { ttl_ = 300; gravity_ = 0.08f; }
    Vec2<float> GetLocation() { return location_; }
    float GetXLocation() { return location_.GetX(); }
    float GetYLocation() { return location_.GetY(); }
    int GetTTL() { return ttl_; }
    void SetTTL(int n) { ttl_ = n; }
    float GetGravity() { return gravity_; }
    void SetGravity(float g) { gravity_ = g; }
    Vec2<double> GetAngle() { return angle_; }
    void SetAngle(Vec2<double> v) { angle_ = v; }
    double GetAngleRotation() { return angle_.GetX(); }
    double GetAngleSpeed() { return angle_.GetY(); }
    void SetAngleRotation(double v) { angle_.SetX(v); }
    void SetAngleSpeed(double v) { angle_.SetY(v); }
    void SetFrameModSpeed(int v) { frame_mod_speed_ = v; }
    int GetFrameModSpeed() { return frame_mod_speed_; }

    void Update() {
      speed_.SetY(speed_.GetY() - gravity_);
      location_ -= speed_;
      angle_.SetX(angle_.GetX() + angle_.GetY());
      --ttl_;
    }

  protected:
    Vec2<float> location_;  // x/y location
    Vec2<float> speed_;     // x/y speed
    Vec2<double> angle_;    // rotation angle/speed
    int frame_mod_speed_;   // frame shown per mod frames
    int ttl_;               // how many frames to live
    float gravity_;         // gravity factor
};

#endif
