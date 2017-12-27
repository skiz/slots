#ifndef STATE_H
#define STATE_H

#include <vector>
#include "engine.h"
#include "signal.h"

class State {
  public:
    virtual void Init(Engine* e) = 0;
    virtual void Cleanup() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
  protected:
    std::vector<int> signal_bindings_;
    State() {}
};

#endif

