#ifndef STATE_H
#define STATE_H

#include "engine.h"

class State {
  public:
    virtual void Init(Engine* e) = 0;
    virtual void Cleanup() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
    virtual void HandleEvents() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;

    /*
    void ChangeState(Engine* e, State* s) {
      e->ChangeState(s);
    }
    */

  protected:
    State() {}
};

#endif

