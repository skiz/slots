#ifndef BLANK_STATE_H
#define BLANK_STATE_H

#include "state.h"

class BlankState : public State {
  public:
    void Init(Engine* e);
    void Cleanup();
    void Pause();
    void Resume();
    void Update();
    void Draw();
    static BlankState* Instance() {
      return &state;
    }
  protected:
    BlankState() {}

  private:
    Engine* engine_; // not owned
    static BlankState state;
};


#endif
