#ifndef FPS_TIMER_H
#define FPS_TIMER_H

#include "timer.h"

class FpsTimer {
  public:
    void Start() { fpsTimer_.Start(); }
    void Tick() { ++frameCount_; }
    void Update() { fps_ = frameCount_ / (fpsTimer_.GetTicks() / 1000.f); };
  private:
    Timer fpsTimer_;
    float fps_ = 0.0;
    int frameCount_;
};

#endif
