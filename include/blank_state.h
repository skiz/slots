#ifndef BLANK_STATE_H
#define BLANK_STATE_H

#include "state.h"

class BlankState : public State {
  public:
    void Init(Engine* e);
    void Cleanup();
    void Pause();
    void Resume();
    void HandleEvents(Engine* e);
    void Update(Engine* e);
    void Draw(Engine* e);
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
